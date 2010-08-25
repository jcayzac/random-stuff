#if !defined(UUID_72E07B6F349C4C7EBE3D21F5A0D67AD2)
#define UUID_72E07B6F349C4C7EBE3D21F5A0D67AD2

#include <string>

class FileSystem {
public:
	enum EDirectory {
		ROOT_DIR=1000,
		CACHE_DIR,
		TEMP_DIR,
		APP_DIR,
		DOC_DIR,
		RES_DIR
	};
public:
	static void set(const std::string& root,
					const std::string& cache,
					const std::string& temp,
					const std::string& app,
					const std::string& doc,
					const std::string& res);
	//
	static std::string root();
	static std::string cacheDir();
	static std::string tempDir();
	static std::string appDir();
	static std::string docDir();
	static std::string resDir();
	
	static std::string buildPath(EDirectory dir, const std::string& path);
	static std::string buildTempName();
};

#endif // UUID_72E07B6F349C4C7EBE3D21F5A0D67AD2

