#version 330 core
out vec4 FragColor;

struct Material { // 当描述一个物体的时候，我们可以用这三个分量来定义一个材质颜色; 我们为每个冯氏光照模型的分量都定义一个颜色向量。
    sampler2D diffuse; // diffuse材质向量定义了在漫反射光照下物体的颜色。（和环境光照一样）漫反射颜色也要设置为我们需要的物体颜色。
    sampler2D specular; // specular材质向量设置的是镜面光照对物体的颜色影响（或者甚至可能反射一个物体特定的镜面高光颜色）   
    float shininess; // 反光度(Shininess);
}; 

struct Light {
    vec3 position;  
  
    vec3 ambient; // ambient材质向量定义了在环境光照下这个物体反射得是什么颜色，通常这是和物体颜色相同的颜色。
    vec3 diffuse; // diffuse材质向量定义了在漫反射光照下物体的颜色。（和环境光照一样）漫反射颜色也要设置为我们需要的物体颜色。
    vec3 specular; // specular材质向量设置的是镜面光照对物体的颜色影响（或者甚至可能反射一个物体特定的镜面高光颜色）。 
	
    float constant; // 为了实现衰减，在片段着色器中我们还需要三个额外的值;
    float linear; // 也就是公式中的常数项、一次项和二次项。
    float quadratic; // 它们最好储存在之前定义的Light结构体中。
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
    vec3 lightDir = normalize(light.position - FragPos); // 注意我们首先对light.direction向量取反。我们目前使用的光照计算需求一个从片段至光源的光线方向，但人们更习惯定义定向光为一个从光源出发的全局方向。
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;  
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;  
    
    // attenuation
    float distance    = length(light.position - FragPos); // 我们可以通过获取片段和光源之间的向量差，并获取结果向量的长度作为距离项。我们可以使用GLSL内建的length函数来完成这一点
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    ambient  *= attenuation; // 接下来，我们将包含这个衰减值到光照计算中，将它分别乘以环境光、漫反射和镜面光颜色。
    diffuse   *= attenuation;
    specular *= attenuation;   
        
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
} 