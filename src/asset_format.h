#pragma once
#include "stdint.h"

#define EPAS_ASSET_MAGIC 0x5453534153415045 //"EPASASST"
#define EPAS_ASSET_LOOP_ANIM_MAGIC 0x4d494e41504f4f4c //"LOOPANIM"
#define EPAS_ASSET_INTRO_ANIM_MAGIC 0x4d494e4152544e49 //"INTRANIM"
#define EPAS_ASSET_OVERLAY_IMG_MAGIC 0x474d4959414c564f //"OVLAYIMG"
#define EPAS_ASSET_INTRO_ICON_MAGIC 0x4e4f434952544e49 //"INTRICON"

typedef struct {
    uint64_t magic; //"EPASASST"
    uint32_t length;
    uint16_t version;
    char name[20];
    char author[20];
    struct {
        uint32_t has_intro_anim:1;
        uint32_t has_overlay_img:1;
        uint32_t has_intro_icon:1;
        uint32_t reserved:29;
    } flag;
    uint32_t loop_anim_offset;
    uint32_t intro_anim_offset;
    uint32_t overlay_img_offset;
    uint32_t intro_icon_offset;
} epass_asset_header_t;

typedef struct {
    uint64_t magic; //"LOOPANIM"
    uint32_t length;
    uint32_t flag; // reserved
    uint16_t frame_count;
} epass_asset_loop_anim_t;

typedef struct {
    uint64_t magic; //"INTRANIM"
    uint32_t length;
    uint32_t flag; // reserved
    uint16_t frame_count;
} epass_asset_intro_anim_t;

typedef struct {
    uint64_t magic; //"OVLAYIMG"
    uint32_t length;
    uint32_t flag;
} epass_asset_overlay_img_t;

typedef struct {
    uint64_t magic; //"INTRICON"
    uint32_t length;
    uint32_t flag; // reserved
} epass_asset_intro_icon_t;

