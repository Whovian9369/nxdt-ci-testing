/*
 * bfttf.c
 *
 * Copyright (c) 2018, simontime.
 * Copyright (c) 2020, DarkMatterCore <pabloacurielz@gmail.com>.
 *
 * This file is part of nxdumptool (https://github.com/DarkMatterCore/nxdumptool).
 *
 * nxdumptool is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * nxdumptool is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "utils.h"
#include "bfttf.h"
#include "romfs.h"
#include "title.h"

/* Type definitions. */

typedef struct {
    u64 title_id;   ///< System title ID.
    char path[64];  ///< Path to BFTTF file inside the RomFS section from the system title.
    u32 size;
    u8 *data;
} BfttfFontInfo;

/* Global variables. */

static Mutex g_bfttfMutex = 0;
static bool g_bfttfInterfaceInit = false;

static BfttfFontInfo g_fontInfo[] = {
    { 0x0100000000000811, "/nintendo_udsg-r_std_003.bfttf", 0, NULL },          /* FontStandard. */
    { 0x0100000000000810, "/nintendo_ext_003.bfttf", 0, NULL },                 /* FontNintendoExtension (1).*/
    { 0x0100000000000810, "/nintendo_ext2_003.bfttf", 0, NULL },                /* FontNintendoExtension (2).*/
    { 0x0100000000000812, "/nintendo_udsg-r_ko_003.bfttf", 0, NULL },           /* FontKorean. */
    { 0x0100000000000814, "/nintendo_udsg-r_org_zh-cn_003.bfttf", 0, NULL },    /* FontChineseSimplified (1). */
    { 0x0100000000000814, "/nintendo_udsg-r_ext_zh-cn_003.bfttf", 0, NULL },    /* FontChineseSimplified (2). */
    { 0x0100000000000813, "/nintendo_udjxh-db_zh-tw_003.bfttf", 0, NULL }       /* FontChineseTraditional. */
};

static const u32 g_fontInfoCount = MAX_ELEMENTS(g_fontInfo);

static const u32 g_bfttfKey = 0x06186249;

/* Function prototypes. */

static bool bfttfDecodeFont(BfttfFontInfo *font_info);

bool bfttfInitialize(void)
{
    mutexLock(&g_bfttfMutex);
    
    u32 count = 0;
    NcaContext *nca_ctx = NULL;
    TitleInfo *title_info = NULL;
    u64 prev_title_id = 0;
    
    RomFileSystemContext romfs_ctx = {0};
    RomFileSystemFileEntry *romfs_file_entry = NULL;
    
    bool ret = g_bfttfInterfaceInit;
    if (ret) goto end;
    
    /* Allocate memory for a temporary NCA context. */
    nca_ctx = calloc(1, sizeof(NcaContext));
    if (!nca_ctx)
    {
        LOGFILE("Failed to allocate memory for temporary NCA context!");
        goto end;
    }
    
    /* Retrieve BFTTF data. */
    for(u32 i = 0; i < g_fontInfoCount; i++)
    {
        BfttfFontInfo *font_info = &(g_fontInfo[i]);
        
        /* Check if the title ID for the current font container matches the one from the previous font container. */
        /* We won't have to reinitialize both NCA and RomFS contexts if that's the case. */
        if (font_info->title_id != prev_title_id)
        {
            /* Get title info. */
            if (!(title_info = titleGetInfoFromStorageByTitleId(NcmStorageId_BuiltInSystem, font_info->title_id)))
            {
                LOGFILE("Failed to get title info for %016lX!", font_info->title_id);
                continue;
            }
            
            /* Initialize NCA context. */
            if (!ncaInitializeContext(nca_ctx, NcmStorageId_BuiltInSystem, 0, titleGetContentInfoByTypeAndIdOffset(title_info, NcmContentType_Data, 0), NULL))
            {
                LOGFILE("Failed to initialize Data NCA context for %016lX!", font_info->title_id);
                continue;
            }
            
            /* Initialize RomFS context. */
            if (!romfsInitializeContext(&romfs_ctx, &(nca_ctx->fs_ctx[0])))
            {
                LOGFILE("Failed to initialize RomFS context for Data NCA from %016lX!", font_info->title_id);
                continue;
            }
            
            /* Update previous title ID. */
            prev_title_id = font_info->title_id;
        }
        
        /* Get RomFS file entry. */
        if (!(romfs_file_entry = romfsGetFileEntryByPath(&romfs_ctx, font_info->path)))
        {
            LOGFILE("Failed to retrieve RomFS file entry in %016lX!", font_info->title_id);
            continue;
        }
        
        /* Check file size. */
        if (!romfs_file_entry->size)
        {
            LOGFILE("Invalid file size for \"%s\" in %016lX!", font_info->path, font_info->title_id);
            continue;
        }
        
        /* Allocate memory for BFTTF data. */
        if (!(font_info->data = malloc(romfs_file_entry->size)))
        {
            LOGFILE("Failed to allocate 0x%lX bytes for \"%s\" in %016lX!", romfs_file_entry->size, font_info->path, font_info->title_id);
            continue;
        }
        
        /* Read BFTFF data. */
        if (!romfsReadFileEntryData(&romfs_ctx, romfs_file_entry, font_info->data, romfs_file_entry->size, 0))
        {
            LOGFILE("Failed to read 0x%lX bytes long \"%s\" in %016lX!", romfs_file_entry->size, font_info->path, font_info->title_id);
            free(font_info->data);
            font_info->data = NULL;
            continue;
        }
        
        /* Update BFTTF size. */
        font_info->size = (u32)romfs_file_entry->size;
        
        /* Decode BFTTF data. */
        if (!bfttfDecodeFont(font_info))
        {
            LOGFILE("Failed to decode 0x%lX bytes long \"%s\" in %016lX!", romfs_file_entry->size, font_info->path, font_info->title_id);
            free(font_info->data);
            font_info->data = NULL;
            font_info->size = 0;
            continue;
        }
        
        /* Increase retrieved BFTTF count. */
        count++;
    }
    
    ret = g_bfttfInterfaceInit = (count > 0);
    if (!ret) LOGFILE("No BFTTF fonts retrieved!");
    
end:
    romfsFreeContext(&romfs_ctx);
    
    if (nca_ctx) free(nca_ctx);
    
    mutexUnlock(&g_bfttfMutex);
    
    return ret;
}

void bfttfExit(void)
{
    mutexLock(&g_bfttfMutex);
    
    /* Free BFTTF data. */
    for(u32 i = 0; i < g_fontInfoCount; i++)
    {
        BfttfFontInfo *font_info = &(g_fontInfo[i]);
        font_info->size = 0;
        if (font_info->data) free(font_info->data);
    }
    
    g_bfttfInterfaceInit = false;
    
    mutexUnlock(&g_bfttfMutex);
}

bool bfttfGetFontByType(BfttfFontData *font_data, u8 font_type)
{
    if (!font_data || font_type >= BfttfFontType_Total)
    {
        LOGFILE("Invalid parameters!");
        return false;
    }
    
    BfttfFontInfo *font_info = &(g_fontInfo[font_type]);
    if (font_info->size <= 8 || !font_info->data)
    {
        LOGFILE("BFTTF font data unavailable for type 0x%02X!", font_type);
        return false;
    }
    
    font_data->type = font_type;
    font_data->size = (font_info->size - 8);
    font_data->ptr = (font_info->data + 8);
    
    return true;
}

static bool bfttfDecodeFont(BfttfFontInfo *font_info)
{
    if (!font_info || font_info->size <= 8 || !IS_ALIGNED(font_info->size, 4) || !font_info->data)
    {
        LOGFILE("Invalid parameters!");
        return false;
    }
    
    for(u32 i = 8; i < (font_info->size - 8); i += 4)
    {
        u32 *ptr = (u32*)(font_info->data + i);
        *ptr = (*ptr ^ g_bfttfKey);
    }
    
    return true;
}