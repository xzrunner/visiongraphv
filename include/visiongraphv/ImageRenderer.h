#pragma once

#include <unirender2/typedef.h>

namespace cv { class Mat; }
namespace ur2 { class Device; class Context; }

namespace vgv
{

class ImageRenderer
{
public:
    void Setup(const ur2::Device& dev,
        const std::shared_ptr<cv::Mat>& mat);

    void Draw(const ur2::Device& dev, ur2::Context& ctx) const;

    void Clear();

private:
    ur2::TexturePtr m_tex = nullptr;

}; // ImageRenderer

}