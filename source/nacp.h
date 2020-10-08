/*
 * nacp.h
 *
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

#pragma once

#ifndef __NACP_H__
#define __NACP_H__

#include "romfs.h"

#define NACP_MAX_ICON_SIZE  0x20000 /* 128 KiB. */

typedef struct {
    char name[0x200];
    char publisher[0x100];
} NacpTitle;

typedef enum {
    NacpStartupUserAccount_None                                       = 0,
    NacpStartupUserAccount_Required                                   = 1,
    NacpStartupUserAccount_RequiredWithNetworkServiceAccountAvailable = 2,
    NacpStartupUserAccount_Count                                      = 3   ///< Total values supported by this enum.
} NacpStartupUserAccount;

typedef enum {
    NacpUserAccountSwitchLock_Disable = 0,
    NacpUserAccountSwitchLock_Enable  = 1,
    NacpUserAccountSwitchLock_Count   = 2   ///< Total values supported by this enum.
} NacpUserAccountSwitchLock;

typedef enum {
    NacpAddOnContentRegistrationType_AllOnLaunch = 0,
    NacpAddOnContentRegistrationType_OnDemand    = 1,
    NacpAddOnContentRegistrationType_Count       = 2    ///< Total values supported by this enum.
} NacpAddOnContentRegistrationType;

typedef enum {
    NacpAttribute_Demo                     = BIT(0),
    NacpAttribute_RetailInteractiveDisplay = BIT(1),
    NacpAttribute_Count                    = 2          ///< Total values supported by this enum.
} NacpAttribute;

/// Indexes used to access NACP Title structs.
typedef enum {
    NacpLanguage_AmericanEnglish      = 0,
    NacpLanguage_BritishEnglish       = 1,
    NacpLanguage_Japanese             = 2,
    NacpLanguage_French               = 3,
    NacpLanguage_German               = 4,
    NacpLanguage_LatinAmericanSpanish = 5,
    NacpLanguage_Spanish              = 6,
    NacpLanguage_Italian              = 7,
    NacpLanguage_Dutch                = 8,
    NacpLanguage_CanadianFrench       = 9,
    NacpLanguage_Portuguese           = 10,
    NacpLanguage_Russian              = 11,
    NacpLanguage_Korean               = 12,
    NacpLanguage_TraditionalChinese   = 13,
    NacpLanguage_SimplifiedChinese    = 14,
    NacpLanguage_BrazilianPortuguese  = 15,
    NacpLanguage_Count                = 16  ///< Total values supported by this enum.
} NacpLanguage;

typedef enum {
    NacpSupportedLanguage_AmericanEnglish      = BIT(0),
    NacpSupportedLanguage_BritishEnglish       = BIT(1),
    NacpSupportedLanguage_Japanese             = BIT(2),
    NacpSupportedLanguage_French               = BIT(3),
    NacpSupportedLanguage_German               = BIT(4),
    NacpSupportedLanguage_LatinAmericanSpanish = BIT(5),
    NacpSupportedLanguage_Spanish              = BIT(6),
    NacpSupportedLanguage_Italian              = BIT(7),
    NacpSupportedLanguage_Dutch                = BIT(8),
    NacpSupportedLanguage_CanadianFrench       = BIT(9),
    NacpSupportedLanguage_Portuguese           = BIT(10),
    NacpSupportedLanguage_Russian              = BIT(11),
    NacpSupportedLanguage_Korean               = BIT(12),
    NacpSupportedLanguage_TraditionalChinese   = BIT(13),   ///< Old: NacpSupportedLanguage_Taiwanese.
    NacpSupportedLanguage_SimplifiedChinese    = BIT(14),   ///< Old: NacpSupportedLanguage_Chinese.
    NacpSupportedLanguage_BrazilianPortuguese  = BIT(15),
    NacpSupportedLanguage_Count                = 16         ///< Total values supported by this enum. Should always match NacpLanguage_Count.
} NacpSupportedLanguage;

typedef enum {
    NacpParentalControl_FreeCommunication = BIT(0),
    NacpParentalControl_Count             = 1       ///< Total values supported by this enum.
} NacpParentalControl;

typedef enum {
    NacpScreenshot_Allow = 0,
    NacpScreenshot_Deny  = 1,
    NacpScreenshot_Count = 2    ///< Total values supported by this enum.
} NacpScreenshot;

typedef enum {
    NacpVideoCapture_Disable = 0,
    NacpVideoCapture_Manual  = 1,
    NacpVideoCapture_Enable  = 2,
    NacpVideoCapture_Count   = 3,                           ///< Total values supported by this enum.
    
    /// Old.
    NacpVideoCapture_Deny    = NacpVideoCapture_Disable,
    NacpVideoCapture_Allow   = NacpVideoCapture_Manual
} NacpVideoCapture;

typedef enum {
    NacpDataLossConfirmation_None     = 0,
    NacpDataLossConfirmation_Required = 1,
    NacpDataLossConfirmation_Count    = 2   ///< Total values supported by this enum.
} NacpDataLossConfirmation;

typedef enum {
    NacpPlayLogPolicy_Open    = 0,
    NacpPlayLogPolicy_LogOnly = 1,
    NacpPlayLogPolicy_None    = 2,
    NacpPlayLogPolicy_Closed  = 3,
    NacpPlayLogPolicy_Count   = 4,                      ///< Total values supported by this enum.
    
    /// Old.
    NacpPlayLogPolicy_All     = NacpPlayLogPolicy_Open
} NacpPlayLogPolicy;

/// Indexes used to access NACP RatingAge info.
typedef enum {
    NacpRatingAgeOrganization_CERO         = 0,
    NacpRatingAgeOrganization_GRACGCRB     = 1,
    NacpRatingAgeOrganization_GSRMR        = 2,
    NacpRatingAgeOrganization_ESRB         = 3,
    NacpRatingAgeOrganization_ClassInd     = 4,
    NacpRatingAgeOrganization_USK          = 5,
    NacpRatingAgeOrganization_PEGI         = 6,
    NacpRatingAgeOrganization_PEGIPortugal = 7,
    NacpRatingAgeOrganization_PEGIBBFC     = 8,
    NacpRatingAgeOrganization_Russian      = 9,
    NacpRatingAgeOrganization_ACB          = 10,
    NacpRatingAgeOrganization_OFLC         = 11,
    NacpRatingAgeOrganization_IARCGeneric  = 12,
    NacpRatingAgeOrganization_Count        = 13     ///< Total values supported by this enum.
} NacpRatingAgeOrganization;

typedef struct {
    u8 cero;
    u8 gracgcrb;
    u8 gsrmr;
    u8 esrb;
    u8 class_ind;
    u8 usk;
    u8 pegi;
    u8 pegi_portugal;
    u8 pegibbfc;
    u8 russian;
    u8 acb;
    u8 oflc;
    u8 iarc_generic;
    u8 reserved[0x13];
} NacpRatingAge;

typedef enum {
    NacpLogoType_LicensedByNintendo    = 0,
    NacpLogoType_DistributedByNintendo = 1, ///< Removed.
    NacpLogoType_Nintendo              = 2,
    NacpLogoType_Count                 = 3  ///< Total values supported by this enum.
} NacpLogoType;

typedef enum {
    NacpLogoHandling_Auto   = 0,
    NacpLogoHandling_Manual = 1,
    NacpLogoHandling_Count  = 2     ///< Total values supported by this enum.
} NacpLogoHandling;

typedef enum {
    NacpRuntimeAddOnContentInstall_Deny                                       = 0,
    NacpRuntimeAddOnContentInstall_AllowAppend                                = 1,
    NacpRuntimeAddOnContentInstall_AllowAppendButDontDownloadWhenUsingNetwork = 2,
    NacpRuntimeAddOnContentInstall_Count                                      = 3   ///< Total values supported by this enum.
} NacpRuntimeAddOnContentInstall;

typedef enum {
    NacpRuntimeParameterDelivery_Always                   = 0,
    NacpRuntimeParameterDelivery_AlwaysIfUserStateMatched = 1,
    NacpRuntimeParameterDelivery_OnRestart                = 2,
    NacpRuntimeParameterDelivery_Count                    = 3   ///< Total values supported by this enum.
} NacpRuntimeParameterDelivery;

typedef enum {
    NacpCrashReport_Deny  = 0,
    NacpCrashReport_Allow = 1,
    NacpCrashReport_Count = 2   ///< Total values supported by this enum.
} NacpCrashReport;

typedef enum {
    NacpHdcp_None     = 0,
    NacpHdcp_Required = 1,
    NacpHdcp_Count    = 2   ///< Total values supported by this enum.
} NacpHdcp;

typedef enum {
    NacpStartupUserAccountOption_IsOptional = BIT(0),
    NacpStartupUserAccountOption_Count      = 1         ///< Total values supported by this enum.
} NacpStartupUserAccountOption;

typedef enum {
    NacpPlayLogQueryCapability_None      = 0,
    NacpPlayLogQueryCapability_WhiteList = 1,
    NacpPlayLogQueryCapability_All       = 2,
    NacpPlayLogQueryCapability_Count     = 3    ///< Total values supported by this enum.
} NacpPlayLogQueryCapability;

typedef enum {
    NacpRepair_SuppressGameCardAccess = BIT(0),
    NacpRepair_Count                  = 1       ///< Total values supported by this enum.
} NacpRepair;

typedef enum {
    NacpRequiredNetworkServiceLicenseOnLaunch_Common = BIT(0),
    NacpRequiredNetworkServiceLicenseOnLaunch_Count  = 1        ///< Total values supported by this enum.
} NacpRequiredNetworkServiceLicenseOnLaunch;

typedef enum {
	NacpJitConfigurationFlag_None    = 0,
	NacpJitConfigurationFlag_Enabled = 1,
    NacpJitConfigurationFlag_Count   = 2    ///< Total values supported by this enum.
} NacpJitConfigurationFlag;

typedef struct {
    u64 jit_configuration_flag; ///< NacpJitConfigurationFlag.
    u64 memory_size;
} NacpJitConfiguration;

typedef struct {
    u16 index        : 15;
    u16 continue_set : 1;   ///< Called "flag" by Nintendo, which isn't really great...
} NacpDescriptors;

typedef struct {
    NacpDescriptors descriptors[0x20];
} NacpRequiredAddOnContentsSetBinaryDescriptor;

typedef enum {
    NacpPlayReportPermission_None            = 0,
    NacpPlayReportPermission_TargetMarketing = 1,
    NacpPlayReportPermission_Count           = 2    ///< Total values supported by this enum.
} NacpPlayReportPermission;

typedef enum {
    NacpCrashScreenshotForProd_Deny  = 0,
    NacpCrashScreenshotForProd_Allow = 1,
    NacpCrashScreenshotForProd_Count = 2    ///< Total values supported by this enum.
} NacpCrashScreenshotForProd;

typedef enum {
    NacpCrashScreenshotForDev_Deny  = 0,
    NacpCrashScreenshotForDev_Allow = 1,
    NacpCrashScreenshotForDev_Count = 2     ///< Total values supported by this enum.
} NacpCrashScreenshotForDev;

typedef struct {
    u64 application_id[8];
} NacpAccessibleLaunchRequiredVersion;

typedef struct {
    NacpTitle title[0x10];
    char isbn[0x25];
    u8 startup_user_account;                                                                        ///< NacpStartupUserAccount.
    u8 user_account_switch_lock;                                                                    ///< NacpUserAccountSwitchLock. Old: touch_screen_usage (None, Supported, Required).
    u8 add_on_content_registration_type;                                                            ///< NacpAddOnContentRegistrationType.
    u32 attribute;                                                                                  ///< NacpAttribute.
    u32 supported_language;                                                                         ///< NacpSupportedLanguage.
    u32 parental_control;                                                                           ///< NacpParentalControl.
    u8 screenshot;                                                                                  ///< NacpScreenshot.
    u8 video_capture;                                                                               ///< NacpVideoCapture.
    u8 data_loss_confirmation;                                                                      ///< NacpDataLossConfirmation.
    u8 play_log_policy;                                                                             ///< NacpPlayLogPolicy.
    u64 presence_group_id;
    NacpRatingAge rating_age;
    char display_version[0x10];
    u64 add_on_content_base_id;
    u64 save_data_owner_id;
    u64 user_account_save_data_size;
    u64 user_account_save_data_journal_size;
    u64 device_save_data_size;
    u64 device_save_data_journal_size;
    u64 bcat_delivery_cache_storage_size;
    char application_error_code_category[0x8];
    u64 local_communication_id[8];
    u8 logo_type;                                                                                   ///< NacpLogoType.
    u8 logo_handling;                                                                               ///< NacpLogoHandling.
    u8 runtime_add_on_content_install;                                                              ///< NacpRuntimeAddOnContentInstall.
    u8 runtime_parameter_delivery;                                                                  ///< NacpRuntimeParameterDelivery.
    u8 reserved_1[0x2];
    u8 crash_report;                                                                                ///< NacpCrashReport.
    u8 hdcp;                                                                                        ///< NacpHdcp.
    u64 seed_for_pseudo_device_id;
    char bcat_passphrase[0x41];
    u8 startup_user_account_option;                                                                 ///< NacpStartupUserAccountOption.
    u8 reserved_2[0x6];
    u64 user_account_save_data_size_max;
    u64 user_account_save_data_journal_size_max;
    u64 device_save_data_size_max;
    u64 device_save_data_journal_size_max;
    u64 temporary_storage_size;
    u64 cache_storage_size;
    u64 cache_storage_journal_size;
    u64 cache_storage_data_and_journal_size_max;
    u16 cache_storage_index_max;
    u8 reserved_3[0x6];
    u64 play_log_queryable_application_id[0x10];
    u8 play_log_query_capability;                                                                   ///< NacpPlayLogQueryCapability.
    u8 repair;                                                                                      ///< NacpRepair.
    u8 program_index;
    u8 required_network_service_license_on_launch;                                                  ///< NacpRequiredNetworkServiceLicenseOnLaunch.
    u8 reserved_4[0x4];
    NacpNeighborDetectionClientConfiguration neighbor_detection_client_configuration;
    NacpJitConfiguration jit_configuration;
    NacpRequiredAddOnContentsSetBinaryDescriptor required_add_on_contents_set_binary_descriptor;
    u8 play_report_permission;                                                                      ///< NacpPlayReportPermission.
    u8 crash_screenshot_for_prod;                                                                   ///< NacpCrashScreenshotForProd.
    u8 crash_screenshot_for_dev;                                                                    ///< NacpCrashScreenshotForDev.
    u8 reserved_5[0x5];
    NacpAccessibleLaunchRequiredVersion accessible_launch_required_version;
    u8 reserved_6[0xBB8];
} _NacpStruct;

typedef struct {
    u8 language;    ///< NacpLanguage.
    u64 icon_size;  ///< JPG icon size. Must not exceed NACP_MAX_ICON_SIZE.
    u8 *icon_data;  ///< Pointer to a dynamically allocated buffer that holds the JPG icon data.
} NacpIconContext;

typedef struct {
    NcaContext *nca_ctx;                        ///< Pointer to the NCA context for the Control NCA from which NACP data is retrieved.
    RomFileSystemContext romfs_ctx;             ///< RomFileSystemContext for the Control NCA FS section #0, which is where the NACP is stored.
    RomFileSystemFileEntry *romfs_file_entry;   ///< RomFileSystemFileEntry for the NACP in the Control NCA FS section #0. Used to generate a RomFileSystemFileEntryPatch if needed.
    RomFileSystemFileEntryPatch nca_patch;      ///< RomFileSystemFileEntryPatch generated if NACP modifications are needed. Used to seamlessly replace Control NCA data while writing it.
                                                ///< Bear in mind that generating a patch modifies the NCA context.
    _NacpStruct *data;                          ///< Pointer to a dynamically allocated buffer that holds the full NACP.
    u8 data_hash[SHA256_HASH_SIZE];             ///< SHA-256 checksum calculated over the whole NACP. Used to determine if NcaHierarchicalSha256Patch generation is truly needed.
    u8 icon_count;                              ///< NACP icon count. May be zero if no icons are available.
    NacpIconContext *icon_ctx;                  ///< Pointer to a dynamically allocated buffer that holds 'icon_count' NACP icon contexts. May be NULL if no icons are available.
    char *authoring_tool_xml;                   ///< Pointer to a dynamically allocated, NULL-terminated buffer that holds AuthoringTool-like XML data. 
                                                ///< This is always NULL unless nacpGenerateAuthoringToolXml() is used on this NacpContext.
    u64 authoring_tool_xml_size;                ///< Size for the AuthoringTool-like XML. This is essentially the same as using strlen() on 'authoring_tool_xml'.
                                                ///< This is always 0 unless nacpGenerateAuthoringToolXml() is used on this NacpContext.
} NacpContext;

/// Initializes a NacpContext using a previously initialized NcaContext (which must belong to a Control NCA).
bool nacpInitializeContext(NacpContext *out, NcaContext *nca_ctx);

/// Generates an AuthoringTool-like XML using information from a previously initialized NacpContext, as well as the Application/Patch version and the required system version.
/// If the function succeeds, XML data and size will get saved to the 'authoring_tool_xml' and 'authoring_tool_xml_size' members from the NacpContext.
bool nacpGenerateAuthoringToolXml(NacpContext *nacp_ctx, u32 version, u32 required_system_version);

/// These functions return pointers to string representations of the input flag/value index (e.g. nacpGetLanguageString(NacpLanguage_AmericanEnglish) -> "AmericanEnglish").
/// If the input flag/value index is invalid, "Unknown" will be returned.
/// If dealing with a bitflag field such as NacpAttribute, NacpSupportedLanguage, etc., the provided value must be a 0-based index to the desired flag and not a bitmask from its enum.
/// (e.g. NacpAttribute_RetailInteractiveDisplay -> Use 1 instead).
const char *nacpGetLanguageString(u8 language); ///< Can also be used for NacpSupportedLanguage flags with values from the NacpLanguage enum.
const char *nacpGetStartupUserAccountString(u8 startup_user_account);
const char *nacpGetUserAccountSwitchLockString(u8 user_account_switch_lock);
const char *nacpGetAddOnContentRegistrationTypeString(u8 add_on_content_registration_type);
const char *nacpGetAttributeString(u8 attribute);
const char *nacpGetParentalControlString(u8 parental_control);
const char *nacpGetScreenshotString(u8 screenshot);
const char *nacpGetVideoCaptureString(u8 video_capture);
const char *nacpGetDataLossConfirmationString(u8 data_loss_confirmation);
const char *nacpGetPlayLogPolicyString(u8 play_log_policy);
const char *nacpGetRatingAgeOrganizationString(u8 rating_age_organization);
const char *nacpGetLogoTypeString(u8 logo_type);
const char *nacpGetLogoHandlingString(u8 logo_handling);
const char *nacpGetRuntimeAddOnContentInstallString(u8 runtime_add_on_content_install);
const char *nacpGetRuntimeParameterDeliveryString(u8 runtime_parameter_delivery);
const char *nacpGetCrashReportString(u8 crash_report);
const char *nacpGetHdcpString(u8 hdcp);
const char *nacpGetStartupUserAccountOptionString(u8 startup_user_account_option);
const char *nacpGetPlayLogQueryCapabilityString(u8 play_log_query_capability);
const char *nacpGetRepairString(u8 repair);
const char *nacpGetRequiredNetworkServiceLicenseOnLaunchString(u8 required_network_service_license_on_launch);
const char *nacpGetJitConfigurationFlagString(u64 jig_configuration_flag);  ///< Uses an input u64 value.
const char *nacpGetPlayReportPermissionString(u8 play_report_permission);
const char *nacpGetCrashScreenshotForProdString(u8 crash_screenshot_for_prod);
const char *nacpGetCrashScreenshotForDevString(u8 crash_screenshot_for_dev);

/// Helper inline functions.

NX_INLINE void nacpFreeContext(NacpContext *nacp_ctx)
{
    if (!nacp_ctx) return;
    
    romfsFreeContext(&(nacp_ctx->romfs_ctx));
    romfsFreeFileEntryPatch(&(nacp_ctx->nca_patch));
    if (nacp_ctx->data) free(nacp_ctx->data);
    
    if (nacp_ctx->icon_ctx)
    {
        for(u8 i = 0; i < nacp_ctx->icon_count; i++)
        {
            if (nacp_ctx->icon_ctx[i].icon_data) free(nacp_ctx->icon_ctx[i].icon_data);
        }
        
        free(nacp_ctx->icon_ctx);
    }
    
    if (nacp_ctx->authoring_tool_xml) free(nacp_ctx->authoring_tool_xml);
    memset(nacp_ctx, 0, sizeof(NacpContext));
}

NX_INLINE bool nacpIsValidIconContext(NacpIconContext *icon_ctx)
{
    return (icon_ctx && icon_ctx->language < NacpLanguage_Count && icon_ctx->icon_size && icon_ctx->icon_data);
}

NX_INLINE bool nacpIsValidContext(NacpContext *nacp_ctx)
{
    if (!nacp_ctx || !nacp_ctx->nca_ctx || !nacp_ctx->romfs_file_entry || !nacp_ctx->data || (!nacp_ctx->icon_count && nacp_ctx->icon_ctx) || (nacp_ctx->icon_count && !nacp_ctx->icon_ctx)) return false;
    
    for(u8 i = 0; i < nacp_ctx->icon_count; i++)
    {
        if (!nacpIsValidIconContext(&(nacp_ctx->icon_ctx[i]))) return false;
    }
    
    return true;
}

NX_INLINE bool nacpIsNcaPatchRequired(NacpContext *nacp_ctx)
{
    if (!nacpIsValidContext(nacp_ctx)) return false;
    u8 tmp_hash[SHA256_HASH_SIZE] = {0};
    sha256CalculateHash(tmp_hash, nacp_ctx->data, sizeof(_NacpStruct));
    return (memcmp(tmp_hash, nacp_ctx->data_hash, SHA256_HASH_SIZE) != 0);
}

NX_INLINE bool nacpGenerateNcaPatch(NacpContext *nacp_ctx)
{
    return (nacpIsValidContext(nacp_ctx) && romfsGenerateFileEntryPatch(&(nacp_ctx->romfs_ctx), nacp_ctx->romfs_file_entry, nacp_ctx->data, sizeof(_NacpStruct), 0, &(nacp_ctx->nca_patch)));
}

#endif /* __NACP_H__ */
