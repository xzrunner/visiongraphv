#pragma once

#include <unirender/Texture.h>

namespace cv { class Mat; }

namespace vgv
{

class ImageRenderer
{
public:
    void Setup(const std::shared_ptr<cv::Mat>& mat);

    void Draw() const;

    void Clear();

private:
    ur::TexturePtr m_tex = nullptr;

}; // ImageRenderer

}