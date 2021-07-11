#pragma once

using std::string;
using std::initializer_list;
using std::unordered_map;
using std::pair;
using namespace vmml;

namespace bones
{
	class GL
	{
	private:
		static android_app* Android(android_app* val = nullptr)
		{
			static android_app* _app;

			if (val != nullptr)
				_app = val;

			return _app;
		}

	public:
		static int32_t Width(int32_t val = -1)
		{
			static int32_t _width;

			if (val != -1)
				_width = val;

			return _width;
		}

		static int32_t Height(int32_t val = -1)
		{
			static int32_t _height;

			if (val != -1)
				_height = val;

			return _height;
		}

		static unordered_map<string, GLUID>& Shaders()
		{
			static unordered_map<string, GLUID> _shaders;
			return _shaders;
		}

		static unordered_map<string, GLUID>& Programs()
		{
			static unordered_map<string, GLUID> _programs;
			return _programs;
		}

		static unordered_map<string, GLID>& AttributeLocations()
		{
			static unordered_map<string, GLID> _attLocs;
			return _attLocs;
		}

		static unordered_map<string, GLID>& UniformLocations()
		{
			static unordered_map<string, GLID> _uniLocs;
			return _uniLocs;
		}

		static EGLDisplay Display(EGLDisplay val = nullptr)
		{
			static EGLDisplay _display;

			if (val != nullptr)
				_display = val;

			return _display;
		}

		static EGLSurface Surface(EGLSurface val = nullptr)
		{
			static EGLSurface _surface;

			if (val != nullptr)
				_surface = val;

			return _surface;
		}

		static EGLContext Context(EGLContext val = nullptr)
		{
			static EGLContext _context;

			if (val != nullptr)
				_context = val;

			return _context;
		}

		static COLOR& BackgroundColor(const COLOR& val = COLOR::DOWN)
		{
			static COLOR _backColor;

			if (val != COLOR::DOWN)
				_backColor = val;

			return _backColor;
		}

	private:
		static bool EndsWith(const string& value, const string& ending)
		{
			if (ending.size() > value.size()) return false;
			return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
		}

	public:
		static bool Init(struct android_app* app)
		{
			try
			{
				Android(app);

				const EGLint attribs[] = {
					EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT_KHR,
					EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
					EGL_BLUE_SIZE, 8,
					EGL_GREEN_SIZE, 8,
					EGL_RED_SIZE, 8,
					EGL_DEPTH_SIZE, 16,
					EGL_SAMPLES, 4,
					EGL_NONE
				};

				EGLint contextAttribs[] =
				{
					EGL_CONTEXT_CLIENT_VERSION, 3,
					EGL_NONE
				};

				EGLint w, h, format;
				EGLint numConfigs;
				EGLConfig config;
				EGLSurface surface;
				EGLContext context;

				EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

				eglInitialize(display, 0, 0);
				eglChooseConfig(display, attribs, &config, 1, &numConfigs);
				eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);
				ANativeWindow_setBuffersGeometry(app->window, 0, 0, format);

				surface = eglCreateWindowSurface(display, config, app->window, NULL);
				context = eglCreateContext(display, config, NULL, contextAttribs);

				if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE)
					return false;

				eglQuerySurface(display, surface, EGL_WIDTH, &w);
				eglQuerySurface(display, surface, EGL_HEIGHT, &h);

				Display(display);
				Context(context);
				Surface(surface);
				Width(w);
				Height(h);

				glEnable(GL_DEPTH_TEST);
				glDepthFunc(GL_LEQUAL);

				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				glViewport(0, 0, w, h);

				if (glGetError() > 0)
					throw;

				return true;
			}
			catch (std::exception* e)
			{
				return false;
			}

			return false;
		}

		static void BeginRefresh()
		{
			if (Display() == nullptr) return;
			auto bgColor = BackgroundColor();
			glClearColor(bgColor.x(), bgColor.y(), bgColor.z(), bgColor.a());
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		static void EndRefresh()
		{
			if (Display() == nullptr) return;
			eglSwapBuffers(Display(), Surface());
		}

		static void Terminate()
		{
			if (Display() != EGL_NO_DISPLAY)
			{
				eglMakeCurrent(Display(), EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

				if (Context() != EGL_NO_CONTEXT)
					eglDestroyContext(Display(), Context());

				if (Surface() != EGL_NO_SURFACE)
					eglDestroySurface(Display(), Surface());

				eglTerminate(Display());
			}

			Display(EGL_NO_DISPLAY);
			Context(EGL_NO_CONTEXT);
			Surface(EGL_NO_SURFACE);
		}

		static bool GetAssetData(const string& fileName, GLCHAR** outBytes, INT* outLength)
		{
			try
			{
				AAssetManager* mgr = Android()->activity->assetManager;
				AAsset* asset = AAssetManager_open(mgr, fileName.c_str(), AASSET_MODE_RANDOM);

				off_t start = 0, length = 0;
				AAsset_openFileDescriptor(asset, &start, &length);
				*outLength = length;

				INT bytesRead = 0;
				INT offset = 0;
				*outBytes = new GLCHAR[length + 1];
				(*outBytes)[length] = 0;

				while ((bytesRead = AAsset_read(asset, *outBytes + offset, BUFSIZ)) > 0)
					offset += bytesRead;

				AAsset_close(asset);
			}
			catch (const std::exception& e)
			{
				return false;
			}

			return true;
		}

		static GLUID CreateShader(const string& name)
		{
			try
			{
				GLUID existingId = GetShaderId(name);
				if (existingId > 0) return existingId;

				GLCHAR *bytes;
				INT length;
				GetAssetData("shaders/" + name, &bytes, &length);

				GLUID id = 0;
				if (EndsWith(name, ".vert"))
					id = glCreateShader(GL_VERTEX_SHADER);
				else if (EndsWith(name, ".frag"))
					id = glCreateShader(GL_FRAGMENT_SHADER);
				else if (EndsWith(name, ".geom"))
					id = glCreateShader(GL_GEOMETRY_SHADER);
				else if (EndsWith(name, ".comp"))
					id = glCreateShader(GL_COMPUTE_SHADER);

				glShaderSource(id, 1, &bytes, &length);
				glCompileShader(id);

				GLint success = GL_FALSE;
				glGetShaderiv(id, GL_COMPILE_STATUS, &success);

				if (success == GL_TRUE)
				{
					Shaders().insert(pair<string, GLUID>(name, id));
					return id;
				}
			}
			catch (const std::exception& e)
			{
				return 0;
			}

			return 0;
		}

		static GLUID GetShaderId(const string& name)
		{
			try
			{
				auto shaders = Shaders();
				auto result = shaders.find(name);

				if (result == shaders.end())
					return 0;
				else
					return result->second;
			}
			catch (const std::exception& e)
			{
				return 0;
			}
		}

		static GLUID CreateProgram(const string& name)
		{
			try
			{
				GLUID existingId = GetProgramId(name);
				if (existingId > 0) return existingId;

				GLUID id = glCreateProgram();

				if (id > 0)
				{
					Programs().insert(pair<string, GLUID>(name, id));
					return id;
				}
			}
			catch (const std::exception& e)
			{
				return 0;
			}

			return 0;
		}

		static GLUID CreateProgram(const string& programName, initializer_list<string> shaderNames)
		{
			GLUID prog = 0;

			if ((prog = GL::GetProgramId(programName)) == NULL_GLUID)
			{
				prog = GL::CreateProgram(programName);

				if (prog != GL::GetProgramId(programName))
					throw;

				for (auto&& shaderName : shaderNames)
				{
					auto shader = GL::CreateShader(shaderName);

					if (!GL::GetShaderId(shaderName))
						throw;

					if (!GL::AttachShader(prog, shader))
						throw;
				}

				if (!GL::LinkProgram(prog))
					throw;

				return prog;
			}
			else
			{
				return prog;
			}

			return 0;
		}

		static GLUID GetProgramId(const string& name)
		{
			try
			{
				auto progs = Programs();
				auto result = progs.find(name);

				if (result == progs.end())
					return 0;
				else
					return result->second;
			}
			catch (const std::exception& e)
			{
				return 0;
			}
		}

		static bool AttachShader(GLUID programId, GLUID shaderId)
		{
			try
			{
				glAttachShader(programId, shaderId);
			}
			catch (const std::exception& e)
			{
				return false;
			}

			return true;
		}

		static bool LinkProgram(GLUID programId)
		{
			try
			{
				glLinkProgram(programId);

				GLint success;
				glGetProgramiv(programId, GL_LINK_STATUS, &success);

				return success == GL_TRUE;
			}
			catch (const std::exception& e)
			{
				return false;
			}

			return true;
		}

		static GLID GetAttribLocation(GLUID programId, const string& name)
		{
			try
			{
				auto attlocs = AttributeLocations();
				auto result = attlocs.find(name);

				if (result == attlocs.end())
				{
					auto loc = glGetAttribLocation(programId, name.c_str());

					if (loc > -1)
					{
						attlocs.insert(pair<string, GLID>(name, loc));
						return loc;
					}
				}
				else
				{
					return result->second;
				}
			}
			catch (const std::exception& e)
			{
				return -1;
			}

			return -1;
		}

		static GLID GetUniformLocation(GLUID programId, const string& name)
		{
			try
			{
				auto unilocs = UniformLocations();
				auto result = unilocs.find(name);

				if (result == unilocs.end())
				{
					auto loc = glGetUniformLocation(programId, name.c_str());

					if (loc > -1)
					{
						unilocs.insert(pair<string, GLID>(name, loc));
						return loc;
					}
				}
				else
				{
					return result->second;
				}
			}
			catch (const std::exception& e)
			{
				return -1;
			}

			return -1;
		}

		static bool GenBuffers(GLSIZE n, GLUID* buffers)
		{
			try
			{
				glGenBuffers(n, buffers);
				return glGetError() == GL_NO_ERROR;
			}
			catch (const std::exception& e)
			{
				return false;
			}

			return false;
		}
	};
}
