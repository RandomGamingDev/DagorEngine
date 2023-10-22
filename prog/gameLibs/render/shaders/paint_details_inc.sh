macro USE_PAINT_DETAILS()
  hlsl(vs) {
    float3 getColorMul(uint hashVal)
    {
      float correction = 4.59479341998814;
      uint paint_palette_col = hashVal;
      uint2 dim;
      paint_details_tex.GetDimensions(dim.x, dim.y);
      dim.x >>= 1;
      uint paint_palette_row = get_paint_palette_row_index().x;
      uint palette_index = get_paint_palette_row_index().y;
      float3 colorMul = paint_details_tex[uint2(paint_palette_col%dim.x + dim.x * palette_index, paint_palette_row % dim.y)].rgb;
      colorMul *= correction;
      return colorMul;
    }
  }
endmacro
