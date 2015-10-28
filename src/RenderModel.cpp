#include "RenderModel.h"

RenderModel::~RenderModel() {
	// Release GPU resources
	glDeleteVertexArrays(1, &m_vertexArray);
	glDeleteProgram(m_shaderProgram);
	glDeleteSamplers(1, &m_sampler);
	glDeleteBuffers(1, &m_texture);
	glDeleteBuffers(1, &m_vertexBuffer);
	glDeleteBuffers(1, &m_indexBuffer);
}

void RenderModel::loadShaders(const char* vertexShaderFilename, 
                              const char* pixelShaderFilename, 
                              const char* geometryShaderFilename) {
	if(!PHYSFS_exists(vertexShaderFilename)) {
		throw std::runtime_error(std::string("Could not find vertex shader: ") + vertexShaderFilename);
	}
	if(!PHYSFS_exists(pixelShaderFilename)) {
		throw std::runtime_error(std::string("Could not find pixel shader: ") + pixelShaderFilename);
	}
	if(geometryShaderFilename != nullptr && !PHYSFS_exists(geometryShaderFilename)) {
		throw std::runtime_error(std::string("Could not find geometry shader: ") + geometryShaderFilename);
	}
	
	// NOTE: It may be possible to find the size by seeking if 
	//       PHYSFS_fileLength fails
	
	// Compile vertex shader
	PHYSFS_File* vertexShaderFile = PHYSFS_openRead(vertexShaderFilename);
	if(!vertexShaderFile) {
		throw std::runtime_error(std::string("Could not open vertex shader: ") + vertexShaderFilename);
	}
	
	PHYSFS_sint64 vsFileSizeLong = PHYSFS_fileLength(vertexShaderFile);
	if(vsFileSizeLong == -1)
		throw std::runtime_error(std::string("Could not determine size of vertex shader: ") + vertexShaderFilename);
	if(vsFileSizeLong > std::numeric_limits<int>::max())
		throw std::runtime_error(std::string("Vertex shader too large: ") + vertexShaderFilename);
	
	int vsFileSize = static_cast<int>(vsFileSizeLong);
	
	char* vsBuffer = new char[vsFileSize];
	int vsBytesRead = PHYSFS_read(vertexShaderFile, vsBuffer, 1, vsFileSize);
	PHYSFS_close(vertexShaderFile);
	if(vsBytesRead < vsFileSize || vsBytesRead == -1) {
		delete[] vsBuffer;
		g_logger->write(Logger::ERROR, PHYSFS_getLastError());
		throw std::runtime_error(std::string("Could not read all of vertex shader: ") + vertexShaderFilename);
	}
	
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	
	glShaderSource(vertexShader, 1, &vsBuffer, &vsFileSize);
	delete[] vsBuffer;
	glCompileShader(vertexShader);
	
	int isVSCompiled;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isVSCompiled);
	if(isVSCompiled == GL_FALSE) {
		int maxLength;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);
		
		g_logger->write(Logger::ERROR, infoLog.data());
		
		glDeleteShader(vertexShader);
		throw std::runtime_error("Failed to compile vertex shader");
	}
	
	
	// Compile pixel shader
	PHYSFS_File* pixelShaderFile = PHYSFS_openRead(pixelShaderFilename);
	if(!pixelShaderFile) {
		throw std::runtime_error(std::string("Could not open pixel shader: ") + pixelShaderFilename);
	}
	
	PHYSFS_sint64 psFileSizeLong = PHYSFS_fileLength(pixelShaderFile);
	if(psFileSizeLong == -1)
		throw std::runtime_error(std::string("Could not determine size of pixel shader: ") + pixelShaderFilename);
	if(psFileSizeLong > std::numeric_limits<int>::max())
		throw std::runtime_error(std::string("Pixel shader too large: ") + pixelShaderFilename);
	
	int psFileSize = static_cast<int>(psFileSizeLong);
	
	char* psBuffer = new char[psFileSize];
	int psBytesRead = PHYSFS_read(pixelShaderFile, psBuffer, 1, psFileSize);
	PHYSFS_close(pixelShaderFile);
	if(psBytesRead < psFileSize || psBytesRead == -1) {
		delete[] psBuffer;
		g_logger->write(Logger::ERROR, PHYSFS_getLastError());
		throw std::runtime_error(std::string("Could not read all of pixel shader: ") + pixelShaderFilename);
	}
	
	unsigned int pixelShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(pixelShader, 1, &psBuffer, &psFileSize);
	delete[] psBuffer;
	glCompileShader(pixelShader);
	
	int isPSCompiled;
	glGetShaderiv(pixelShader, GL_COMPILE_STATUS, &isPSCompiled);
	if(isPSCompiled == GL_FALSE) {
		int maxLength;
		glGetShaderiv(pixelShader, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(pixelShader, maxLength, &maxLength, &infoLog[0]);
		
		g_logger->write(Logger::ERROR, infoLog.data());
		
		glDeleteShader(pixelShader);
		throw std::runtime_error("Failed to compile pixel shader");
	}
	
	
	// Compile geometry shader
	unsigned int geometryShader;
	if(geometryShaderFilename != nullptr) {
		PHYSFS_File* geometryShaderFile = PHYSFS_openRead(geometryShaderFilename);
		if(!geometryShaderFile) {
			throw std::runtime_error(std::string("Could not open geometry shader: ") + geometryShaderFilename);
		}
		
		PHYSFS_sint64 gsFileSizeLong = PHYSFS_fileLength(geometryShaderFile);
		if(gsFileSizeLong == -1)
			throw std::runtime_error(std::string("Could not determine size of geometry shader: ") + geometryShaderFilename);
		if(gsFileSizeLong > std::numeric_limits<int>::max())
			throw std::runtime_error(std::string("Geometry shader too large: ") + geometryShaderFilename);
		
		int gsFileSize = static_cast<int>(gsFileSizeLong);
		
		char* gsBuffer = new char[gsFileSize];
		int gsBytesRead = PHYSFS_read(geometryShaderFile, gsBuffer, 1, gsFileSize);
		PHYSFS_close(geometryShaderFile);
		if(gsBytesRead < gsFileSize || gsBytesRead == -1) {
			delete[] gsBuffer;
			g_logger->write(Logger::ERROR, PHYSFS_getLastError());
			throw std::runtime_error(std::string("Could not read all of geometry shader: ") + geometryShaderFilename);
		}
		
		geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
		
		glShaderSource(vertexShader, 1, &gsBuffer, &gsFileSize);
		delete[] gsBuffer;
		glCompileShader(geometryShader);
		
		int isGSCompiled;
		glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &isGSCompiled);
		if(isGSCompiled == GL_FALSE) {
			int maxLength;
			glGetShaderiv(geometryShader, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(geometryShader, maxLength, &maxLength, &infoLog[0]);
			
			g_logger->write(Logger::ERROR, infoLog.data());
			
			glDeleteShader(geometryShader);
			throw std::runtime_error("Failed to compile geometry shader");
		}
	}
	
	
	// Link shaders
	m_shaderProgram = glCreateProgram();
	
	glAttachShader(m_shaderProgram, vertexShader);
	glAttachShader(m_shaderProgram, pixelShader);
	if(geometryShaderFilename != nullptr) {
		glAttachShader(m_shaderProgram, geometryShader);
	}
	
	glLinkProgram(m_shaderProgram);
	
	int isLinked;
	glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &isLinked);
	if(isLinked == GL_FALSE) {
		int maxLength = 0;
		glGetProgramiv(m_shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(m_shaderProgram, maxLength, &maxLength, &infoLog[0]);
		
		g_logger->write(Logger::ERROR, infoLog.data());
		
		glDeleteProgram(m_shaderProgram);
		glDeleteShader(vertexShader);
		glDeleteShader(pixelShader);
		if(geometryShaderFilename != nullptr) {
			glDeleteShader(geometryShader);
		}
		throw std::runtime_error("Failed to link shader program");
	}
	
	glDetachShader(m_shaderProgram, vertexShader);
	glDetachShader(m_shaderProgram, pixelShader);
	if(geometryShaderFilename != nullptr) {
		glDetachShader(m_shaderProgram, geometryShader);
	}
}

unsigned int RenderModel::loadTextureToGPU(const char* filename, int* baseWidth, int* baseHeight) {
	// Use PhysFS to read exture file into memory
	if(std::string(filename).compare("") == 0 || !PHYSFS_exists(filename))
		throw std::runtime_error(std::string("Could not find texture: ") + filename);
	
	PHYSFS_File* file = PHYSFS_openRead(filename);
	if(!file)
		throw std::runtime_error(std::string("Could not open texture: ") + filename);
	
	PHYSFS_sint64 fileSize = PHYSFS_fileLength(file);
	if(fileSize == -1)
		throw std::runtime_error(std::string("Could not determine size of texture: ") + filename);
	
	char* buffer = new char[fileSize];
	int bytesRead = PHYSFS_read(file, buffer, 1, fileSize);
	PHYSFS_close(file);
	if(bytesRead < fileSize || bytesRead == -1) {
		delete[] buffer;
		g_logger->write(Logger::ERROR, PHYSFS_getLastError());
		throw std::runtime_error(std::string("Could not read all of texture: ") + filename);
	}
	
	// Create a GLI texture from the data read in by PhysFS
	gli::texture texture = gli::load(buffer, fileSize);
	delete[] buffer;
	gli::gl gl;
	const gli::gl::format format = gl.translate(texture.format());
	GLenum target = gl.translate(texture.target());
	if(target != GL_TEXTURE_2D) {
		throw std::runtime_error("Texture target is not GL_TEXTURE_2D/gli::TARGET_2D");
	}
	if(texture.empty()) {
		throw std::runtime_error(filename + std::string(" is not a valid texture"));
	}

	// We currently need to write these out in order to size
	// 2d render models. This should be written out in the
	// near future.
	*baseWidth = texture.dimensions().x;
	*baseHeight = texture.dimensions().y;
	
	// Reserve memory on the GPU for texture and describe its layout
	unsigned int textureID;
	glGenTextures(1, &textureID);
	
	glBindTexture(target, textureID);
	
	glTexParameteri(target, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(target, GL_TEXTURE_MAX_LEVEL, texture.levels()-1);
	glTexParameteri(target, GL_TEXTURE_SWIZZLE_R, format.Swizzle[0]);
	glTexParameteri(target, GL_TEXTURE_SWIZZLE_G, format.Swizzle[1]);
	glTexParameteri(target, GL_TEXTURE_SWIZZLE_B, format.Swizzle[2]);
	glTexParameteri(target, GL_TEXTURE_SWIZZLE_A, format.Swizzle[3]);
	
	glTexStorage2D(target, 
	               texture.levels(), 
	               format.Internal,
	               texture.dimensions().x, 
	               texture.dimensions().y);

	// Write image data to GPU memory
	for(unsigned int layer=0; layer<texture.layers(); ++layer) {
		for(unsigned int face=0; face<texture.faces(); ++face) {
			for(unsigned int level=0; level<texture.levels(); ++level) {
				if(gli::is_compressed(texture.format())) {
					glCompressedTexSubImage2D(target, 
					                          level, 
					                          0, 
					                          0, 
					                          texture.dimensions().x, 
					                          texture.dimensions().y, 
					                          format.Internal, 
					                          texture.size(level), 
					                          texture.data(layer, face, level));
				}
				else {
					glTexSubImage2D(target, 
					                level, 
					                0, 
					                0, 
					                texture.dimensions().x, 
					                texture.dimensions().y, 
					                format.External, 
					                format.Type, 
					                texture.data(layer, face, level));
				}
			}
		}
	}

	return textureID;
}
