#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal; // ���������Ѿ���ÿ�����������һ���������������˶�����ɫ�������ǻ�Ҫ���¶�������ָ�롣

out vec3 FragPos;
out vec3 Normal; // ���й��յļ��㶼����Ƭ����ɫ������У�����������Ҫ���������ɶ�����ɫ�����ݵ�Ƭ����ɫ����

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;  
    
    gl_Position = projection * view * vec4(FragPos, 1.0); // ����ڶ�����ɫ���к�������ɣ�������������һ�������������������������ռ�����;
}