#version 330 core
out vec4 FragColor;

in vec3 Normal; // ����������Ƭ����ɫ���ж�����Ӧ���������
in vec3 FragPos; // �����Ƭ����ɫ���������Ӧ�����������
  
uniform vec3 lightPos; // ���ڹ�Դ��λ����һ����̬���������ǿ��Լ򵥵���Ƭ����ɫ���а�������Ϊuniform
uniform vec3 viewPos; // Ϊ�˵õ��۲��ߵ�����ռ����꣬���Ǽ򵥵�ʹ������������λ��������棨����Ȼ���ǹ۲��ߣ����������ǰ���һ��uniform��ӵ�Ƭ����ɫ��������Ӧ�������λ�����괫��Ƭ����ɫ����
uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
    // ambient ��������(Ambient Lighting)����ʹ�ںڰ�������£�������ͨ��Ҳ��Ȼ��һЩ������������Զ���Ĺ⣩���������弸����Զ��������ȫ�ڰ��ġ�Ϊ��ģ����������ǻ�ʹ��һ���������ճ���������Զ�������һЩ��ɫ��
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse ���������(Diffuse Lighting)��ģ���Դ������ķ�����Ӱ��(Directional Impact)�����Ƿ��Ϲ���ģ�����Ӿ����������ķ����������ĳһ����Խ�������Ź�Դ�����ͻ�Խ����
    vec3 norm = normalize(Normal); // ����ͬ��ϣ��ȷ����������������ת��Ϊ��λ�������������ǰѷ��ߺ����յķ������������б�׼����
    vec3 lightDir = normalize(lightPos - FragPos); // ��ķ��������ǹ�Դλ��������Ƭ��λ������֮������������ܼǵ��ڱ任�̳��У������ܹ��򵥵�ͨ����������������ķ�ʽ���������
    float diff = max(dot(norm, lightDir), 0.0); // ����ʹ��max����������������֮��ϴ�Ĳ������Ӷ���֤��������������ɸ�����
    vec3 diffuse = diff * lightColor;
    
    // specular �������(Specular Lighting)��ģ���й�������������ֵ����㡣������յ���ɫ������������ɫ��������ڹ����ɫ��
    float specularStrength = 0.5; // ���ȣ����Ƕ���һ������ǿ��(Specular Intensity)������������߹�һ���е�������ɫ��������Ҫ�������ȵ�Ӱ�졣
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm); // reflect����Ҫ���һ�������Ǵӹ�Դָ��Ƭ��λ�õ�����������lightDir��ǰ�����෴���Ǵ�Ƭ��ָ���Դ������ǰ���Ǽ���lightDir����ʱ��������˳�������;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32); // �����ȼ������߷����뷴�䷽��ĵ�ˣ���ȷ�������Ǹ�ֵ����Ȼ��ȡ����32���ݡ�
    vec3 specular = specularStrength * spec * lightColor;  
        

	// ���Ϲ���ģ�͵���Ҫ�ṹ��3��������ɣ�����(Ambient)��������(Diffuse)�;���(Specular)���ա�
    vec3 result = (ambient + diffuse + specular) * objectColor; // ʣ�µ����һ�������ǰ����ӵ�������������������������ý�������������ɫ
    FragColor = vec4(result, 1.0);
} 