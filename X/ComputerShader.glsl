#version 430 core
// 最大支持宽度为512的图像
layout(local_size_x = 512, local_size_y = 1, local_size_z = 1) in;
// 要进行检测的图像
layout(rgba32f, binding = 0) uniform image2D input_image;
// 检测结果
layout(rgba32f, binding = 1) uniform image2D output_image;
// 共享数组，存储当前行的像素
shared vec4 scanline[512];

void main(void)
{
	// 请求的位置
	ivec2 pos = ivec2(gl_GlobalInvocationID.xy);
	// 读取当前位置的像素
	scanline[pos.x] = imageLoad(input_image, pos);
	// 等待所有请求都走到这里
	barrier();
	// 计算边缘值，存储到output_image
	vec4 result = scanline[min(pos.x + 1, 511)] - scanline[max(pos.x - 1, 0)];
	// pos.yx：把输出图像翻转，这样就可以使用同一compute shader进行2维卷积。
	imageStore(output_image, pos.yx, result);
}