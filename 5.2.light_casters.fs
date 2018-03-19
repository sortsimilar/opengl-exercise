#version 330 core
out vec4 FragColor;

struct Material { // ������һ�������ʱ�����ǿ���������������������һ��������ɫ; ����Ϊÿ�����Ϲ���ģ�͵ķ���������һ����ɫ������
    sampler2D diffuse; // diffuse����������������������������������ɫ�����ͻ�������һ������������ɫҲҪ����Ϊ������Ҫ��������ɫ��
    sampler2D specular; // specular�����������õ��Ǿ�����ն��������ɫӰ�죨�����������ܷ���һ�������ض��ľ���߹���ɫ��   
    float shininess; // �����(Shininess);
}; 

struct Light {
    vec3 position;  
  
    vec3 ambient; // ambient���������������ڻ���������������巴�����ʲô��ɫ��ͨ�����Ǻ�������ɫ��ͬ����ɫ��
    vec3 diffuse; // diffuse����������������������������������ɫ�����ͻ�������һ������������ɫҲҪ����Ϊ������Ҫ��������ɫ��
    vec3 specular; // specular�����������õ��Ǿ�����ն��������ɫӰ�죨�����������ܷ���һ�������ض��ľ���߹���ɫ���� 
	
    float constant; // Ϊ��ʵ��˥������Ƭ����ɫ�������ǻ���Ҫ���������ֵ;
    float linear; // Ҳ���ǹ�ʽ�еĳ����һ����Ͷ����
    float quadratic; // ������ô�����֮ǰ�����Light�ṹ���С�
};

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;
  
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
    // ambient
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos); // ע���������ȶ�light.direction����ȡ��������Ŀǰʹ�õĹ��ռ�������һ����Ƭ������Դ�Ĺ��߷��򣬵����Ǹ�ϰ�߶��嶨���Ϊһ���ӹ�Դ������ȫ�ַ���
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;  
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;  
    
    // attenuation
    float distance    = length(light.position - FragPos); // ���ǿ���ͨ����ȡƬ�κ͹�Դ֮������������ȡ��������ĳ�����Ϊ��������ǿ���ʹ��GLSL�ڽ���length�����������һ��
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    ambient  *= attenuation; // �����������ǽ��������˥��ֵ�����ռ����У������ֱ���Ի����⡢������;������ɫ��
    diffuse   *= attenuation;
    specular *= attenuation;   
        
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
} 