#include <3d/dag_drv3d.h>
#include <3d/dag_drv3d_platform.h>

void d3d::prepare_for_destroy() {}
bool d3d::is_window_occluded() { return false; }
bool d3d::should_use_compute_for_image_processing(std::initializer_list<unsigned>) { return false; }
void d3d::discard_managed_textures() {}
Texture *d3d::get_secondary_backbuffer_tex() { return nullptr; }

// some display related functionality

bool d3d::setgamma(float) { return false; }
void d3d::change_screen_aspect_ratio(float) {}
bool d3d::get_vrr_supported() { return false; }

// samplers

d3d::SamplerHandle d3d::create_sampler(const d3d::SamplerInfo & /*sampler_info*/) { return 0; }
void d3d::destroy_sampler(d3d::SamplerHandle /*sampler*/) {}

void d3d::set_sampler(unsigned /*shader_stage*/, unsigned /*slot*/, d3d::SamplerHandle /*sampler*/) {}

// mesh shaders

void d3d::dispatch_mesh(uint32_t /* thread_group_x */, uint32_t /* thread_group_y */, uint32_t /* thread_group_z */)
{
  G_ASSERT_RETURN(!"VK: dispatch_mesh not implemented yet", );
}

void d3d::dispatch_mesh_indirect(Sbuffer * /* args */, uint32_t /* dispatch_count */, uint32_t /* stride_bytes */,
  uint32_t /* byte_offset */)
{
  G_ASSERT_RETURN(!"VK: dispatch_mesh not implemented yet", );
}

void d3d::dispatch_mesh_indirect_count(Sbuffer * /*args*/, uint32_t /*args_stride_bytes*/, uint32_t /* args_byte_offset */,
  Sbuffer * /* count */, uint32_t /* count_byte_offset */, uint32_t /* max_count */)
{
  G_ASSERT_RETURN(!"VK: dispatch_mesh not implemented yet", );
}

// pipe fences

GPUFENCEHANDLE d3d::insert_fence(GpuPipeline /*gpu_pipeline*/) { return 0; }
void d3d::insert_wait_on_fence(GPUFENCEHANDLE & /*fence*/, GpuPipeline /*gpu_pipeline*/) {}

// multi views

bool d3d::setviews(dag::ConstSpan<Viewport> /* viewports */)
{
  G_ASSERTF(false, "Not implemented");
  return false;
}

bool d3d::setscissors(dag::ConstSpan<ScissorRect> /* scissorRects */)
{
  G_ASSERTF(false, "Not implemented");
  return false;
}

// AA & iOS like MSAA

bool d3d::setantialias(int aa_type) { return aa_type == 0; }
int d3d::getantialias() { return 0; }
bool d3d::set_msaa_pass() { return true; }
bool d3d::set_depth_resolve() { return true; }

// deprecated stuff

bool d3d::isVcolRgba() { return true; }

// old/debug PC dev specific stuff

#if _TARGET_PC_WIN
FSHADER d3d::create_pixel_shader_hlsl(const char *, unsigned, const char *, const char *, String *)
{
  G_ASSERT(false);
  return BAD_PROGRAM;
}

VPROG d3d::create_vertex_shader_hlsl(const char *, unsigned, const char *, const char *, String *)
{
  G_ASSERT(false);
  return BAD_PROGRAM;
}

bool d3d::pcwin32::set_capture_full_frame_buffer(bool /*ison*/) { return false; }
void d3d::pcwin32::set_present_wnd(void *) {}
void *d3d::pcwin32::get_native_surface(BaseTexture *) { return nullptr; }
#endif

// variable shading rate

void d3d::set_variable_rate_shading(unsigned, unsigned, VariableRateShadingCombiner, VariableRateShadingCombiner)
{
  // needs VK_NV_shading_rate_image to support it
  // or VK_KHR_variable_rate_shading
}
void d3d::set_variable_rate_shading_texture(BaseTexture *)
{
  // needs VK_NV_shading_rate_image to support it
  // or VK_KHR_variable_rate_shading
}

// resource/memory heaps

ResourceAllocationProperties d3d::get_resource_allocation_properties(const ResourceDescription & /* desc */) { return {}; }
ResourceHeap *d3d::create_resource_heap(ResourceHeapGroup * /* heap_group */, size_t /* size */, ResourceHeapCreateFlags /* flags */)
{
  return nullptr;
}
void d3d::destroy_resource_heap(ResourceHeap * /* heap */) {}
Sbuffer *d3d::place_buffere_in_resource_heap(ResourceHeap * /* heap */, const ResourceDescription & /* desc */, size_t /* offset */,
  const ResourceAllocationProperties & /* alloc_info */, const char * /* name */)
{
  return nullptr;
}
BaseTexture *d3d::place_texture_in_resource_heap(ResourceHeap * /* heap */, const ResourceDescription & /* desc */,
  size_t /* offset */, const ResourceAllocationProperties & /* alloc_info */, const char * /* name */)
{
  return nullptr;
}
ResourceHeapGroupProperties d3d::get_resource_heap_group_properties(ResourceHeapGroup * /* heap_group */) { return {}; }

// tiled (partial-resident) resources

void d3d::map_tile_to_resource(BaseTexture * /* tex */, ResourceHeap * /* heap */, const TileMapping * /* mapping */,
  size_t /* mapping_count */)
{}
TextureTilingInfo d3d::get_texture_tiling_info(BaseTexture * /* tex */, size_t /* subresource */) { return TextureTilingInfo{}; }
