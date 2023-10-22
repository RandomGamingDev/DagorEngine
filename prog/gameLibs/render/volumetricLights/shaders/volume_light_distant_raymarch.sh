
include "shader_global.sh"
include "use_volumetric_light.sh"
include "viewVecVS.sh"

include "heightmap_common.sh"

include "volume_light_distant_common.sh"

include "volume_light_hardcoded_media.sh"





texture clouds_shadows_2d;
float4 nbs_world_pos_to_clouds_alt__inv_clouds_weather_size__neg_clouds_thickness_m;
float4 clouds_origin_offset;
float4 clouds_hole_pos;

// statistical cloud shadows, without relying on preshader
macro USE_FOG_SHADOWS(code)
  (code){
    nbs_world_pos_to_clouds_alt__inv_clouds_weather_size__neg_clouds_thickness_m@f4 =
      nbs_world_pos_to_clouds_alt__inv_clouds_weather_size__neg_clouds_thickness_m;
    skies_primary_sun_light_dir@f3 = skies_primary_sun_light_dir;
    clouds_origin_offset@f3 = clouds_origin_offset;
    clouds_hole_pos@f4 = clouds_hole_pos;
    clouds_shadows_2d@smp2d = clouds_shadows_2d;
  }
  hlsl(code) {
    #include <atmosphere/statistical_clouds_shadow.hlsli>
  }
endmacro



texture prev_distant_fog_raymarch_start_weights;

int distant_fog_use_smart_raymarching_pattern = 1;
interval distant_fog_use_smart_raymarching_pattern: off<1, on;

macro USE_RAYMARCH_JITTERED_TC(code, frame_id)
  hlsl(code){
    uint calc_raymarch_noise_index(uint2 dtId)
    {
      return calc_raymarch_noise_index(dtId, (uint)frame_id);
    }
  }
endmacro

macro INIT_TRANSFORMED_ZNZFAR_STAGE(code)
  INIT_ZNZFAR_STAGE(code)
  hlsl(code){
    float4 get_transformed_zn_zfar()
    {
      return zn_zfar;
    }
  }
endmacro

macro USE_BASE_AMBIENT_COLOR(code)
  INIT_SIMPLE_AMBIENT(code)
  hlsl(code){
    float3 get_base_ambient_color()
    {
      return amb_color;
    }
  }
endmacro



float4 distant_fog_local_view_z;
float4 distant_fog_raymarch_resolution;
float4 distant_fog_reconstruction_resolution;

float4 distant_fog_raymarch_params_0;
float4 distant_fog_raymarch_params_1;
float4 distant_fog_raymarch_params_2;


float distant_fog_raymarch_step_size = 32;
float distant_fog_smart_raymarching_pattern_depth_bias = 2.0;

int OutputDebug_no = 3;
int OutputRaymarchStartWeights_no = 4;
int OutputDist_no = 5;
int OutputExtinction_no = 6;
int OutputInscatter_no = 7;

shader distant_fog_raymarch_cs
{
  supports global_const_block;
  (cs) {
    inv_volfog_froxel_volume_res@f3 = (inv_volfog_froxel_volume_res.x,inv_volfog_froxel_volume_res.y,inv_volfog_froxel_volume_res.z,0)
    distant_fog_local_view_z@f3 = distant_fog_local_view_z;
    distant_fog_raymarch_resolution@f4 = distant_fog_raymarch_resolution;
    distant_fog_reconstruction_resolution@f4 = distant_fog_reconstruction_resolution;
    world_view_pos@f4 = (world_view_pos.x, world_view_pos.y, world_view_pos.z, time_phase(0, 0));
    distant_fog_raymarch_step_size@f1 = (distant_fog_raymarch_step_size);
    prev_distant_fog_raymarch_start_weights@smp2d = prev_distant_fog_raymarch_start_weights;
    distant_fog_smart_raymarching_pattern_depth_bias@f1 = (distant_fog_smart_raymarching_pattern_depth_bias);
    distant_fog_raymarch_params_0@f4 = distant_fog_raymarch_params_0;
    distant_fog_raymarch_params_1@f4 = distant_fog_raymarch_params_1;
    distant_fog_raymarch_params_2@f4 = distant_fog_raymarch_params_2;
    volfog_blended_slice_start_depth@f1 = (volfog_blended_slice_start_depth);
  }
  VIEW_VEC_OPTIMIZED(cs)
  INIT_TRANSFORMED_ZNZFAR_STAGE(cs)
  INIT_HALF_RES_DEPTH(cs)
  USE_RAYMARCH_FRAME_INDEX(cs)
  INIT_JITTER_PARAMS(cs)
  VOLUME_DEPTH_CONVERSION(cs)
  USE_PREV_TC(cs)
  USE_RAYMARCH_JITTERED_TC(cs, jitter_ray_offset.w)
  INIT_AND_USE_VOLFOG_MODIFIERS(cs)
  GET_MEDIA_HARDCODED(cs)
  USE_BASE_AMBIENT_COLOR(cs)
  ENABLE_ASSERT(cs)

  USE_FOG_SHADOWS(cs)

  if (distant_fog_use_smart_raymarching_pattern == off)
  {
    hlsl(cs) {
      #define DISTANT_FOG_DISABLE_SMART_RAYMARCHING_PATTERN 1
    }
  }

  (cs) {
    OutputRaymarchStartWeights@uav: register(OutputRaymarchStartWeights_no) hlsl {
      RWTexture2D<float2> OutputRaymarchStartWeights@uav;
    }
    OutputDist@uav: register(OutputDist_no) hlsl {
      RWTexture2D<float> OutputDist@uav;
    }
    OutputExtinction@uav: register(OutputExtinction_no) hlsl {
      RWTexture2D<float> OutputExtinction@uav;
    }
    OutputInscatter@uav: register(OutputInscatter_no) hlsl {
      RWTexture2D<float4> OutputInscatter@uav;// .a is 2 bits == raymarching pattern offset
    }
    OutputDebug@uav : register(OutputDebug_no) hlsl {
      #define DECLARE_OUTPUT_DEBUG RWTexture2D<float4> OutputDebug@uav;
    }
  }

  hlsl(cs) {
#if DEBUG_DISTANT_FOG_RAYMARCH
    DECLARE_OUTPUT_DEBUG;
#endif

    float3 calcViewVec(float2 screen_tc)
    {
      return getViewVecOptimized(screen_tc);
    }

    #include "volume_lights_raymarch.hlsl"

    [numthreads( RAYMARCH_WARP_SIZE, RAYMARCH_WARP_SIZE, 1 )]
    void RayMarchDistantFog( uint2 dtId : SV_DispatchThreadID, uint2 tid : SV_GroupThreadID )
    {
      DistantFogRaymarchResult distantFogRaymarchResult = accumulateFog(dtId, tid);

      if (any(dtId >= (uint2)distant_fog_raymarch_resolution.xy))
        return;

      OutputInscatter[dtId] = distantFogRaymarchResult.inscatter;
      OutputExtinction[dtId] = distantFogRaymarchResult.extinction;
      OutputDist[dtId] = distantFogRaymarchResult.weightedEndDist;
      OutputRaymarchStartWeights[dtId] = float2(distantFogRaymarchResult.fogStart, distantFogRaymarchResult.raymarchStepWeight);

#if DEBUG_DISTANT_FOG_RAYMARCH
      OutputDebug[dtId] = distantFogRaymarchResult.debug;
#endif
    }
  }
  compile("target_cs", "RayMarchDistantFog");
}
