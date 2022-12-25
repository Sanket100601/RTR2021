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

  float freq = 10.0f;
  float w = sinf(freq * u + animTime) * cosf(freq * v + animTime) * 0.02f;
  /*float freq = 10.0f;
  float w = 1 - abs(sinf(freq * u + time)) * abs(cosf(freq * v + time) * abs(cosf(freq * v + time)) * 0.4f);*/

  pos[y * width + x] = make_float4(u, w, v, 1.0f);
  return;
}

void launchCUDAKernel(float4 *pos, unsigned int width, unsigned int height,
                      float animTime) {
  dim3 block(8, 8, 1);
  dim3 grid(width / block.x, height / block.y, 1);

  sinewave<<<grid, block>>>(pos, width, height, animTime);
}