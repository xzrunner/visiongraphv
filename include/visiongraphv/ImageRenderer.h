#pragma once

#include <unirender/typedef.h>

namespace cv { class Mat; }
namespace ur { class Device; class Context; }

namespace vgv
{

class ImageRenderer
{
public:
    void Setup(const ur::Device& dev,
        const std::shared_ptr<cv::Mat>& mat);

    void Draw(const ur::Device& dev, ur::Context& ctx) const;

    void Clear();

private:
    ur::TexturePtr m_tex = nullptr;

}; // ImageRenderer

}