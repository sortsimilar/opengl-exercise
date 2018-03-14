

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n" // 下一步，使用in关键字，在顶点着色器中声明所有的输入顶点属性(Input Vertex Attribute)。现在我们只关心位置(Position)数据，所以我们只需要一个顶点属性。
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n" // 为了设置顶点着色器的输出，我们必须把位置数据赋值给预定义的gl_Position变量，它在幕后是vec4类型的。在main函数的最后，我们将gl_Position设置的值会成为该顶点着色器的输出。由于我们的输入是一个3分量的向量，我们必须把它转换为4分量的。我们可以把vec3的数据作为vec4构造器的参数，同时把w分量设置为1.0f（我们会在后面解释为什么）来完成这一任务。
"}\0"; // 我们已经写了一个顶点着色器源码（储存在一个C的字符串中），但是为了能够让OpenGL使用它，我们必须在运行时动态编译它的源码。
const char *fragmentShaderSource_1 = "#version 330 core\n"
"out vec4 FragColor;\n" // 我们可以用out关键字声明输出变量，这里我们命名为FragColor。
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";
const char *fragmentShaderSource_2 = "#version 330 core\n"
"out vec4 FragColor;\n" // 我们可以用out关键字声明输出变量，这里我们命名为FragColor。
"void main()\n"
"{\n"
"   FragColor = vec4(0.5f, 0.5f, 0.5f, 1.0f);\n"
"}\n\0";

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit(); // 我们在main函数中调用glfwInit函数来初始化GLFW;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // 我们将主版本号(Major)和次版本号(Minor)都设为3;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // 我们将主版本号(Major)和次版本号(Minor)都设为3;
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 我们同样明确告诉GLFW我们使用的是核心模式(Core-profile);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

														 // glfw window creation
														 // --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL); // glfwCreateWindow函数需要窗口的宽和高作为它的前两个参数。第三个参数表示这个窗口的名称（标题），这里我们使用"LearnOpenGL";
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate(); // 当渲染循环结束后我们需要正确释放/删除之前的分配的所有资源。我们可以在main函数的最后调用glfwTerminate函数来完成。
		return -1;
	}
	glfwMakeContextCurrent(window); // 创建完窗口我们就可以通知GLFW将我们窗口的上下文设置为当前线程的主上下文了;
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // 我们还需要注册这个函数，告诉GLFW我们希望每当窗口调整大小的时候调用这个函数;

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	// build and compile our shader program
	// ------------------------------------
	// vertex shader
	// 我们把需要创建的着色器类型以参数形式提供给glCreateShader。由于我们正在创建一个顶点着色器，传递的参数是GL_VERTEX_SHADER。
	int vertexShader = glCreateShader(GL_VERTEX_SHADER); // 我们首先要做的是创建一个着色器对象，注意还是用ID来引用的。所以我们储存这个顶点着色器为unsigned int，然后用glCreateShader创建这个着色器;
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // glShaderSource函数把要编译的着色器对象作为第一个参数。第二参数指定了传递的源码字符串数量，这里只有一个。第三个参数是顶点着色器真正的源码，第四个参数我们先设置为NULL。
	glCompileShader(vertexShader); // 下一步我们把这个着色器源码附加到着色器对象上，然后编译它;
	// check for shader compile errors
	// 你可能会希望检测在调用glCompileShader后编译是否成功了，如果没成功的话，你还会希望知道错误是什么，这样你才能修复它们。
	int success; // 首先我们定义一个整型变量来表示是否成功编译;
	char infoLog[512]; // 还定义了一个储存错误消息（如果有的话）的容器。
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); // 然后我们用glGetShaderiv检查是否编译成功。
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog); // 如果编译失败，我们会用glGetShaderInfoLog获取错误消息，然后打印它。
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// fragment shader
	int fragmentShader_1 = glCreateShader(GL_FRAGMENT_SHADER); // 编译片段着色器的过程与顶点着色器类似，只不过我们使用GL_FRAGMENT_SHADER常量作为着色器类型;
	int fragmentShader_2 = glCreateShader(GL_FRAGMENT_SHADER); // 编译片段着色器的过程与顶点着色器类似，只不过我们使用GL_FRAGMENT_SHADER常量作为着色器类型;
	glShaderSource(fragmentShader_1, 1, &fragmentShaderSource_1, NULL);
	glShaderSource(fragmentShader_2, 1, &fragmentShaderSource_2, NULL);
	glCompileShader(fragmentShader_1);
	glCompileShader(fragmentShader_2);
	// check for shader compile errors
	glGetShaderiv(fragmentShader_1, GL_COMPILE_STATUS, &success);
	glGetShaderiv(fragmentShader_2, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader_1, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	// 着色器程序对象(Shader Program Object)是多个着色器合并之后并最终链接完成的版本。如果要使用刚才编译的着色器我们必须把它们链接(Link)为一个着色器程序对象，然后在渲染对象的时候激活这个着色器程序。已激活着色器程序的着色器将在我们发送渲染调用的时候被使用。
	// link shaders
	int shaderProgram_1 = glCreateProgram(); // glCreateProgram函数创建一个程序，并返回新创建程序对象的ID引用。
	int shaderProgram_2 = glCreateProgram(); // glCreateProgram函数创建一个程序，并返回新创建程序对象的ID引用。
	glAttachShader(shaderProgram_1, vertexShader); // 现在我们需要把之前编译的着色器附加到程序对象上，然后用glLinkProgram链接它们;
	glAttachShader(shaderProgram_2, vertexShader); // 现在我们需要把之前编译的着色器附加到程序对象上，然后用glLinkProgram链接它们;
	glAttachShader(shaderProgram_1, fragmentShader_1);
	glAttachShader(shaderProgram_2, fragmentShader_2);
	glLinkProgram(shaderProgram_1); // glCreateProgram函数创建一个程序，并返回新创建程序对象的ID引用。现在我们需要把之前编译的着色器附加到程序对象上，然后用glLinkProgram链接它们;
	glLinkProgram(shaderProgram_2); // glCreateProgram函数创建一个程序，并返回新创建程序对象的ID引用。现在我们需要把之前编译的着色器附加到程序对象上，然后用glLinkProgram链接它们;
	// check for linking errors
	glGetProgramiv(shaderProgram_1, GL_LINK_STATUS, &success); // 就像着色器的编译一样，我们也可以检测链接着色器程序是否失败，并获取相应的日志。
	if (!success) {
		glGetProgramInfoLog(shaderProgram_1, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader); // 在把着色器对象链接到程序对象以后，记得删除着色器对象，我们不再需要它们了;
	glDeleteShader(fragmentShader_1);

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// 由于我们希望渲染一个三角形，我们一共要指定三个顶点，每个顶点都有一个3D位置。我们会将它们以标准化设备坐标的形式（OpenGL的可见区域）定义为一个float数组。
	float vertices_1[] = {
		-1.0f, -1.0f, 0.0f, // left  
		1.0f, -1.0f, 0.0f, // right 
		0.0f,  0.0f, 0.0f
	};

	float vertices_2[] = {
		-1.0f, 1.0f, 0.0f, // left  
		1.0f, 1.0f, 0.0f, // right 
		0.0f,  0.0f, 0.0f
	};

	// glBufferData是一个专门用来把用户定义的数据复制到当前绑定缓冲的函数。它的第一个参数是目标缓冲的类型：顶点缓冲对象当前绑定到GL_ARRAY_BUFFER目标上。第二个参数指定传输数据的大小(以字节为单位)；用一个简单的sizeof计算出顶点数据大小就行。第三个参数是我们希望发送的实际数据。
	unsigned int VBO_1, VAO_1;
	glGenVertexArrays(1, &VAO_1); // 创建一个VAO_1和创建一个VBO_1很类似;
	glGenBuffers(1, &VBO_1); // 顶点缓冲对象是我们在OpenGL教程中第一个出现的OpenGL对象。就像OpenGL中的其它对象一样，这个缓冲有一个独一无二的ID，所以我们可以使用glGenBuffers函数和一个缓冲ID生成一个VBO对象;
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO_1); // 要想使用VAO_1，要做的只是使用glBindVertexArray绑定VAO_1。

	glBindBuffer(GL_ARRAY_BUFFER, VBO_1); // OpenGL允许我们同时绑定多个缓冲，只要它们是不同的缓冲类型。我们可以使用glBindBuffer函数把新创建的缓冲绑定到GL_ARRAY_BUFFER目标上;
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_1), vertices_1, GL_STATIC_DRAW); // 从这一刻起，我们使用的任何（在GL_ARRAY_BUFFER目标上的）缓冲调用都会用来配置当前绑定的缓冲(VBO)。然后我们可以调用glBufferData函数，它会把之前定义的顶点数据复制到缓冲的内存中

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // 有了这些信息我们就可以使用glVertexAttribPointer函数告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上）了;
	glEnableVertexAttribArray(0); // 现在我们已经定义了OpenGL该如何解释顶点数据，我们现在应该使用glEnableVertexAttribArray，以顶点属性位置值作为参数，启用顶点属性；

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO_1 afterwards so other VAO_1 calls won't accidentally modify this VAO_1, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);
	//----------------------------------------------------------------------------------------------------
	unsigned int VBO_2, VAO_2;
	glGenVertexArrays(1, &VAO_2); // 创建一个VAO_1和创建一个VBO_1很类似;
	glGenBuffers(1, &VBO_2); // 顶点缓冲对象是我们在OpenGL教程中第一个出现的OpenGL对象。就像OpenGL中的其它对象一样，这个缓冲有一个独一无二的ID，所以我们可以使用glGenBuffers函数和一个缓冲ID生成一个VBO对象;
							 // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO_2); // 要想使用VAO_1，要做的只是使用glBindVertexArray绑定VAO_1。

	glBindBuffer(GL_ARRAY_BUFFER, VBO_2); // OpenGL允许我们同时绑定多个缓冲，只要它们是不同的缓冲类型。我们可以使用glBindBuffer函数把新创建的缓冲绑定到GL_ARRAY_BUFFER目标上;
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_2), vertices_2, GL_STATIC_DRAW); // 从这一刻起，我们使用的任何（在GL_ARRAY_BUFFER目标上的）缓冲调用都会用来配置当前绑定的缓冲(VBO)。然后我们可以调用glBufferData函数，它会把之前定义的顶点数据复制到缓冲的内存中

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // 有了这些信息我们就可以使用glVertexAttribPointer函数告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上）了;
	glEnableVertexAttribArray(0); // 现在我们已经定义了OpenGL该如何解释顶点数据，我们现在应该使用glEnableVertexAttribArray，以顶点属性位置值作为参数，启用顶点属性；

								  // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO_1 afterwards so other VAO_1 calls won't accidentally modify this VAO_1, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	//---------------------------------------------------------------------------------------------------
	// uncomment this call to draw in wireframe polygons.
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// 我们需要在程序中添加一个while循环，我们可以把它称之为渲染循环(Render Loop)，它能在我们让GLFW退出前一直保持运行。
	while (!glfwWindowShouldClose(window)) // glfwWindowShouldClose函数在我们每次循环的开始前检查一次GLFW是否被要求退出，如果是的话该函数返回true然后渲染循环便结束了，之后为我们就可以关闭应用程序了。
	{
		// input
		// -----
		processInput(window); // 我们接下来在渲染循环的每一个迭代中调用processInput;

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // 除了glClear之外，我们还调用了glClearColor来设置清空屏幕所用的颜色。当调用glClear函数，清除颜色缓冲之后，整个颜色缓冲都会被填充为glClearColor里所设置的颜色。在这里，我们将屏幕设置为了类似黑板的深蓝绿色。
		glClear(GL_COLOR_BUFFER_BIT); // 我们可以通过调用glClear函数来清空屏幕的颜色缓冲，它接受一个缓冲位(Buffer Bit)来指定要清空的缓冲，可能的缓冲位有GL_COLOR_BUFFER_BIT，GL_DEPTH_BUFFER_BIT和GL_STENCIL_BUFFER_BIT。由于现在我们只关心颜色值，所以我们只清空颜色缓冲。

		// draw our first triangle
		glUseProgram(shaderProgram_1); // 我们可以调用glUseProgram函数，用刚创建的程序对象作为它的参数，以激活这个程序对象;
		glBindVertexArray(VAO_1); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawArrays(GL_TRIANGLES, 0, 3); // glDrawArrays函数第一个参数是我们打算绘制的OpenGL图元的类型。由于我们在一开始时说过，我们希望绘制的是一个三角形，这里传递GL_TRIANGLES给它。第二个参数指定了顶点数组的起始索引，我们这里填0。最后一个参数指定我们打算绘制多少个顶点，这里是3（我们只从我们的数据中渲染一个三角形，它只有3个顶点长）。
		// glBindVertexArray(0); // no need to unbind it every time 

		// draw our first triangle
		glUseProgram(shaderProgram_2); // 我们可以调用glUseProgram函数，用刚创建的程序对象作为它的参数，以激活这个程序对象;
		glBindVertexArray(VAO_2); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawArrays(GL_TRIANGLES, 0, 3); // glDrawArrays函数第一个参数是我们打算绘制的OpenGL图元的类型。由于我们在一开始时说过，我们希望绘制的是一个三角形，这里传递GL_TRIANGLES给它。第二个参数指定了顶点数组的起始索引，我们这里填0。最后一个参数指定我们打算绘制多少个顶点，这里是3（我们只从我们的数据中渲染一个三角形，它只有3个顶点长）。
		// glBindVertexArray(0); // no need to unbind it every time 


		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window); // glfwSwapBuffers函数会交换颜色缓冲（它是一个储存着GLFW窗口每一个像素颜色值的大缓冲），它在这一迭代中被用来绘制，并且将会作为输出显示在屏幕上。
		glfwPollEvents(); // glfwPollEvents函数检查有没有触发什么事件（比如键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数（可以通过回调方法手动设置）;
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO_1);
	glDeleteBuffers(1, &VBO_1);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate(); // 当渲染循环结束后我们需要正确释放/删除之前的分配的所有资源。我们可以在main函数的最后调用glfwTerminate函数来完成。
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) // 我们同样也希望能够在GLFW中实现一些输入控制，这可以通过使用GLFW的几个输入函数来完成。我们将会使用GLFW的glfwGetKey函数，它需要一个窗口以及一个按键作为输入。这里我们检查用户是否按下了返回键(Esc)（如果没有按下，glfwGetKey将会返回GLFW_RELEASE。如果用户的确按下了返回键，我们将通过glfwSetwindowShouldClose使用把WindowShouldClose属性设置为 true的方法关闭GLFW。下一次while循环的条件检测将会失败，程序将会关闭。
		glfwSetWindowShouldClose(window, true); // 这个函数将会返回这个按键是否正在被按下。我们将创建一个processInput函数来让所有的输入代码保持整洁。
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// 然而，当用户改变窗口的大小的时候，视口也应该被调整。我们可以对窗口注册一个回调函数(Callback Function)，它会在每次窗口大小被调整的时候被调用。
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height); // 在我们开始渲染之前还有一件重要的事情要做，我们必须告诉OpenGL渲染窗口的尺寸大小，即视口(Viewport)，这样OpenGL才只能知道怎样根据窗口大小显示数据和坐标。我们可以通过调用glViewport函数来设置窗口的维度(Dimension);
}

