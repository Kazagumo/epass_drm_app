#include "asset_format.h"
#pragma once

#define EPAS_ASSET_VERSION 0

typedef struct {
    epass_asset_header_t* header;
    epass_asset_loop_anim_t* loop_anim;
    epass_asset_intro_anim_t* intro_anim;
    epass_asset_overlay_img_t* overlay_img;
    epass_asset_intro_icon_t* intro_icon;

    int fd;
    uint32_t *loop_anim_frame_locs;
    uint32_t *intro_anim_frame_locs;

    uint32_t loop_anim_data_offset;
    uint32_t intro_anim_data_offset;
    uint32_t overlay_img_data_offset;
    uint32_t intro_icon_data_offset;

} epass_asset_runtime_info_t;

int load_asset(const char *asset_path, epass_asset_runtime_info_t *runtime_info);
