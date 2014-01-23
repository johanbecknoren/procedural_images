#ifndef _FBO_H_
#define _FBO_H_

//#include <GL/glew.h>
//#include <GL/glfw.h>


class Fbo {
public:
    Fbo(){};
    Fbo(int width, int height, int int_method);
    ~Fbo(){};

    //static void useDepthFbo(Fbo *out, Fbo *in);

	const int getWidth() { return width; }
	const int getHeight() { return height; }

    static void useFbo(Fbo *out, const Fbo *in, const  Fbo *in2);
    static void useArrayFbo(Fbo *out, const Fbo *in);
    static void useFbo(Fbo *out, const  Fbo *in, const  Fbo *in2, const  Fbo *in3);
    static void useDepthFbo(Fbo *out, const Fbo *in);

    const unsigned int getDepthId() const { return depth; };
    const unsigned int getTexId() const { return texid; };

private:
    unsigned int texid;
    unsigned int fb;
    unsigned int rb;
    unsigned int depth;
    int width, height;

    static int lastw;
    static int lasth;

};
#endif