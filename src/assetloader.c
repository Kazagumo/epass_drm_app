#include "assetloader.h"
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "log.h"


int load_asset(const char *asset_path, epass_asset_runtime_info_t *runtime_info){
    int fd = open(asset_path, O_RDONLY);
    if(fd < 0)
    {
        log_error("open asset file failed");
        return -1;
    }

    runtime_info->fd = fd;

    runtime_info->header = malloc(sizeof(epass_asset_header_t));
    if(runtime_info->header == NULL)
    {
        log_error("malloc header failed");
        return -1;
    }

    read(fd, runtime_info->header, sizeof(epass_asset_header_t));
    if(runtime_info->header->magic != EPAS_ASSET_MAGIC)
    {
        log_error("header magic is not correct");
        return -1;
    }

    if(runtime_info->header->version != EPAS_ASSET_VERSION)
    {
        log_error("asset version is not supported. expected: %d, got: %d", EPAS_ASSET_VERSION, runtime_info->header->version);
        return -1;
    }

    runtime_info->loop_anim = malloc(sizeof(epass_asset_loop_anim_t));
    if(runtime_info->loop_anim == NULL)
    {
        log_error("malloc loop anim failed");
        return -1;
    }
    lseek(fd, runtime_info->header->loop_anim_offset, SEEK_SET);
    read(fd, runtime_info->loop_anim, sizeof(epass_asset_loop_anim_t));
    if(runtime_info->loop_anim->magic != EPAS_ASSET_LOOP_ANIM_MAGIC)
    {
        log_error("loop anim magic is not correct");
        return -1;
    }

    log_info("loop anim frame count: %d", runtime_info->loop_anim->frame_count);

    runtime_info->loop_anim_frame_locs = malloc(sizeof(uint32_t) * runtime_info->loop_anim->frame_count);
    if(runtime_info->loop_anim_frame_locs == NULL)
    {
        log_error("malloc loop anim frame locs failed");
        return -1;
    }

    read(fd, runtime_info->loop_anim_frame_locs, sizeof(uint32_t) * runtime_info->loop_anim->frame_count);
    runtime_info->loop_anim_data_offset = lseek(fd, 0, SEEK_CUR);
    
    if(runtime_info->header->flag.has_intro_anim)
    {
        runtime_info->intro_anim = malloc(sizeof(epass_asset_intro_anim_t));
        if(runtime_info->intro_anim == NULL)
        {
            log_error("malloc intro anim failed");
            return -1;
        }
        lseek(fd, runtime_info->header->intro_anim_offset, SEEK_SET);
        read(fd, runtime_info->intro_anim, sizeof(epass_asset_intro_anim_t));
        if(runtime_info->intro_anim->magic != EPAS_ASSET_INTRO_ANIM_MAGIC)
        {
            log_error("intro anim magic is not correct");
            return -1;
        }
        log_info("intro anim frame count: %d", runtime_info->intro_anim->frame_count);
        runtime_info->intro_anim_frame_locs = malloc(sizeof(uint32_t) * runtime_info->intro_anim->frame_count);
        if(runtime_info->intro_anim_frame_locs == NULL)
        {
            log_error("malloc intro anim frame locs failed");
            return -1;
        }
        read(fd, runtime_info->intro_anim_frame_locs, sizeof(uint32_t) * runtime_info->intro_anim->frame_count);
        runtime_info->intro_anim_data_offset = lseek(fd, 0, SEEK_CUR);
    }

    if(runtime_info->header->flag.has_overlay_img)
    {
        runtime_info->overlay_img = malloc(sizeof(epass_asset_overlay_img_t));
        if(runtime_info->overlay_img == NULL)
        {
            log_error("malloc overlay img failed");
            return -1;
        }
        lseek(fd, runtime_info->header->overlay_img_offset, SEEK_SET);
        read(fd, runtime_info->overlay_img, sizeof(epass_asset_overlay_img_t));
        if(runtime_info->overlay_img->magic != EPAS_ASSET_OVERLAY_IMG_MAGIC)
        {
            log_error("overlay img magic is not correct");
            return -1;
        }
        log_info("overlay img data length: %d", runtime_info->overlay_img->length);
        runtime_info->overlay_img_data_offset = lseek(fd, 0, SEEK_CUR);
    }

    if(runtime_info->header->flag.has_intro_icon)
    {
        runtime_info->intro_icon = malloc(sizeof(epass_asset_intro_icon_t));
        if(runtime_info->intro_icon == NULL)
        {
            log_error("malloc intro icon failed");
            return -1;
        }
    }
    lseek(fd, runtime_info->header->intro_icon_offset, SEEK_SET);
    read(fd, runtime_info->intro_icon, sizeof(epass_asset_intro_icon_t));
    if(runtime_info->intro_icon->magic != EPAS_ASSET_INTRO_ICON_MAGIC)
    {
        log_error("intro icon magic is not correct");
        return -1;
    }
    log_info("intro icon data length: %d", runtime_info->intro_icon->length);
    runtime_info->intro_icon_data_offset = lseek(fd, 0, SEEK_CUR);

    return 0;
}

#define FREE_AND_NULL(ptr) do { if(ptr != NULL) { free(ptr); ptr = NULL; } } while(0)

int free_asset(epass_asset_runtime_info_t *runtime_info)
{
    FREE_AND_NULL(runtime_info->header);
    FREE_AND_NULL(runtime_info->loop_anim);
    FREE_AND_NULL(runtime_info->loop_anim_frame_locs);
    FREE_AND_NULL(runtime_info->intro_anim);
    FREE_AND_NULL(runtime_info->intro_anim_frame_locs);
    FREE_AND_NULL(runtime_info->overlay_img);
    FREE_AND_NULL(runtime_info->intro_icon);
    close(runtime_info->fd);

    return 0;
}

void log_asset_info(epass_asset_runtime_info_t *runtime_info)
{
    log_info("asset name: %s", runtime_info->header->name);
    log_info("asset author: %s", runtime_info->header->author);
    log_info("asset version: %d", runtime_info->header->version);
    log_info("asset header length: %d", runtime_info->header->length);
    log_info("loop anim frame count: %d", runtime_info->loop_anim->frame_count);
    if(runtime_info->header->flag.has_intro_anim)
    {
        log_info("intro anim frame count: %d", runtime_info->intro_anim->frame_count);
    }
    if(runtime_info->header->flag.has_overlay_img)
    {
        log_info("overlay img data length: %d", runtime_info->overlay_img->length);
    }
    if(runtime_info->header->flag.has_intro_icon)
    {
        log_info("intro icon data length: %d", runtime_info->intro_icon->length);
    }
}