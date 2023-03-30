// sinewave kernel
#include<iostream>
#include<math.h>
#include<stdio.h>
__global__ void sinewave(float4 *pos, unsigned int width, unsigned int height,
                         float animTime) {
  unsigned int x = blockIdx.x * blockDim.x + threadIdx.x;
  unsigned int y = blockIdx.y * blockDim.y + threadIdx.y;

  float u = x / (float)width;
  float v = y / (float)height;

  u = (u * 2.0) - 1.0;
  v = (v * 2.0) - 1.0;

 /* float freq = 10.0f;
  float w = sinf(freq * u + animTime) * cosf(freq * v + animTime) * 0.02f;*/
  float t = 0.01 * (-animTime * 130.0);
  float freq = 6.0f;
  float amp = 1.0f;

  float w = (sinf(freq * (u * v) + t)) * 0.1  /** sinf(freq * v + t)*/;
  w += (sinf((freq * 2.1 * (u * v)) + t)) * 0.492f;
  w += (sinf((freq * 1.72 * (u * v)) + (t * 1.121))) * 0.4f;
  w += (sinf((freq * 2.221 * (u * v)) + (t * 0.437))) * 0.5f;
  w += (sinf((freq * 3.1122 * (u * v)) + (t * 4.269))) * 0.25f;
  w *= amp * 0.3;

  pos[y * width + x] = make_float4(u, w, v, 1.0f);
  return;
}

void launchCUDAKernel(float4 *pos, unsigned int width, unsigned int height,
                      float animTime) {
  dim3 block(8, 8, 1);
  dim3 grid(width / block.x, height / block.y, 1);

  sinewave<<<grid, block>>>(pos, width, height, animTime);
}