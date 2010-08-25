#if !defined(TEXTURE_H_INCLUDED)
#define TEXTURE_H_INCLUDED
#include <string>

class TextureT {
public:
	TextureT(): mId(0) { }
	bool init(const std::string& path);
	void bind(int slot) const;
	unsigned int getId() const { return mId; }
	~TextureT();
private:
	unsigned int mId;
};

#endif // TEXTURE_H_INCLUDED
