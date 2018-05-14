#version 430 core
// ���֧�ֿ��Ϊ512��ͼ��
layout(local_size_x = 512, local_size_y = 1, local_size_z = 1) in;
// Ҫ���м���ͼ��
layout(rgba32f, binding = 0) uniform image2D input_image;
// �����
layout(rgba32f, binding = 1) uniform image2D output_image;
// �������飬�洢��ǰ�е�����
shared vec4 scanline[512];

void main(void)
{
	// �����λ��
	ivec2 pos = ivec2(gl_GlobalInvocationID.xy);
	// ��ȡ��ǰλ�õ�����
	scanline[pos.x] = imageLoad(input_image, pos);
	// �ȴ����������ߵ�����
	barrier();
	// �����Եֵ���洢��output_image
	vec4 result = scanline[min(pos.x + 1, 511)] - scanline[max(pos.x - 1, 0)];
	// pos.yx�������ͼ��ת�������Ϳ���ʹ��ͬһcompute shader����2ά�����
	imageStore(output_image, pos.yx, result);
}