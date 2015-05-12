#ifndef IMAGE_HINT_H
#define IMAGE_HINT_H
#include <string>
#include "lms/imaging/draw_debug.h"
#include <vector>
namespace lms{
namespace imaging{
namespace find{

class ImageHintBase{

    bool m_processed;
protected:
    bool m_success;

public:
    std::string name;
    virtual ~ImageHintBase(){
    }
    virtual bool find(DRAWDEBUG_PARAM_N)=0;

    void processed(bool processed){
        m_processed = processed;
    }

    bool processed(){
        return m_processed;
    }

    bool success(){
        return m_success;
    }

};

template<class T>
class ImageHint: public ImageHintBase{
public:

    T imageObject;
    typename T::parameterType parameter;

    bool find(DRAWDEBUG_PARAM_N){
        m_success = imageObject.find(parameter DRAWDEBUG_ARG);
        return m_success;
    }
};

class HintContainer{
public:
    std::string name;
    std::vector<ImageHintBase*> hints;

    void add(ImageHintBase *hint){
        hints.push_back(hint);
    }

    void reset(){
        hints.clear();
    }

    ImageHintBase* getByName(const std::string &name) {
        for(ImageHintBase *hint : hints) {
            if(hint->name == name) {
                return hint;
            }
        }

        return nullptr;
    }

    const ImageHintBase* getByName(const std::string &name) const {
        for(ImageHintBase *hint : hints) {
            if(hint->name == name) {
                return hint;
            }
        }

        return nullptr;
    }
};
}
}
}
#endif
