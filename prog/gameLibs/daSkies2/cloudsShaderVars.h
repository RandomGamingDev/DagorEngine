#pragma once

#define CLOUDS_VARS_LIST                                                                  \
  VAR(cloud_volume_res, false)                                                            \
  VAR(clouds2_current_frame, true)                                                        \
  VAR(clouds2_dispatch_groups, false)                                                     \
  VAR(clouds2_resolution, true)                                                           \
  VAR(clouds_ambient_desaturation, false)                                                 \
  VAR(clouds_average_weight, false)                                                       \
  VAR(clouds_back_eccentricity, false)                                                    \
  VAR(clouds_close_layer_is_outside, true)                                                \
  VAR(clouds_color, true)                                                                 \
  VAR(clouds_color_close, true)                                                           \
  VAR(clouds_color_prev, true)                                                            \
  VAR(clouds_compute_width, true)                                                         \
  VAR(clouds_cumulonimbus_shape_scale, true)                                              \
  VAR(clouds_depth, true)                                                                 \
  VAR(clouds_depth_gbuf, true)                                                            \
  VAR(clouds_detail_tex_size, false)                                                      \
  VAR(clouds_epicness, false)                                                             \
  VAR(clouds_erosion_noise_tile_size, false)                                              \
  VAR(clouds_erosion_noise_wind_ofs, true)                                                \
  VAR(clouds_field_downsample_ratio, false)                                               \
  VAR(clouds_field_res, false)                                                            \
  VAR(clouds_field_volume, false)                                                         \
  VAR(clouds_first_layer_density, false)                                                  \
  VAR(clouds_forward_eccentricity, false)                                                 \
  VAR(clouds_forward_eccentricity_weight, false)                                          \
  VAR(clouds_gen_mips_3d_one_layer, false)                                                \
  VAR(clouds_gen_mips_3d_source, false)                                                   \
  VAR(clouds_has_close_sequence, false)                                                   \
  VAR(clouds_height_fractions, false)                                                     \
  VAR(clouds_hole_light_dir, true)                                                        \
  VAR(clouds_hole_target_alt, true)                                                       \
  VAR(clouds_hole_density, true)                                                          \
  VAR(clouds_infinite_skies, true)                                                        \
  VAR(clouds_layer1_coverage, false)                                                      \
  VAR(clouds_layer1_freq, false)                                                          \
  VAR(clouds_layer1_seed, false)                                                          \
  VAR(clouds_layer2_coverage, false)                                                      \
  VAR(clouds_layer2_freq, false)                                                          \
  VAR(clouds_layer2_seed, false)                                                          \
  VAR(clouds_layers_types, false)                                                         \
  VAR(clouds_ms_attenuation, false)                                                       \
  VAR(clouds_ms_contribution, false)                                                      \
  VAR(clouds_ms_eccentricity_attenuation, false)                                          \
  VAR(clouds_new_shadow_ambient_weight, true)                                             \
  VAR(clouds_offset, true)                                                                \
  VAR(clouds_origin_offset, true)                                                         \
  VAR(clouds_prev_depth_gbuf, true)                                                       \
  VAR(clouds_prev_taa_weight, true)                                                       \
  VAR(clouds_rain_clouds_amount, false)                                                   \
  VAR(clouds_rain_clouds_seed, false)                                                     \
  VAR(clouds_restart_taa, true)                                                           \
  VAR(clouds_second_layer_density, false)                                                 \
  VAR(clouds_shadow_coverage, false)                                                      \
  VAR(clouds_shape_scale, false)                                                          \
  VAR(clouds_shape_tex_size_xz, false)                                                    \
  VAR(clouds_shape_tex_size_y, false)                                                     \
  VAR(clouds_start_altitude2, false)                                                      \
  VAR(clouds_start_compute_offset, true)                                                  \
  VAR(clouds_target_depth_gbuf, true)                                                     \
  VAR(clouds_target_depth_gbuf_transform, true)                                           \
  VAR(clouds_thickness2, false)                                                           \
  VAR(clouds_tile_distance, true)                                                         \
  VAR(clouds_tile_distance_tmp, true)                                                     \
  VAR(clouds_tiled_res, true)                                                             \
  VAR(clouds_turbulence_freq, false)                                                      \
  VAR(clouds_turbulence_scale, false)                                                     \
  VAR(clouds_weather_size, false)                                                         \
  VAR(clouds_wind_alt_gradient, true)                                                     \
  VAR(clouds_use_fullres, true)                                                           \
  VAR(compress_voltex_bc4_source, false)                                                  \
  VAR(dispatch_size, false)                                                               \
  VAR(global_clouds_sigma, false)                                                         \
  VAR(nbs_world_pos_to_clouds_alt__inv_clouds_weather_size__neg_clouds_thickness_m, true) \
  VAR(clouds_hole_pos, false)                                                             \
  VAR(nbs_clouds_start_altitude2_meters, true)


#define VAR(a, opt) extern int a##VarId;
CLOUDS_VARS_LIST
#undef VAR
