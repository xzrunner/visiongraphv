#include "visiongraphv/ImageRenderer.h"

#include <unirender2/Device.h>
#include <unirender2/TextureDescription.h>
#include <unirender2/RenderState.h>
#include <unirender2/Factory.h>
#include <painting2/RenderSystem.h>
#include <renderpipeline/RenderMgr.h>

#include <opencv2/opencv.hpp>

namespace vgv
{

void ImageRenderer::Setup(const ur2::Device& dev,
                          const std::shared_ptr<cv::Mat>& mat)
{
    if (!mat || mat->empty()) {
        return;
    }

    auto t = mat->type();

    ur2::TextureDescription desc;
    desc.target = ur2::TextureTarget::Texture2D;
    desc.width  = mat->cols;
    desc.height = mat->rows;

    switch (mat->type())
    {
    case CV_8U:
    {
        uint8_t* pixel_ptr = (uint8_t*)mat->data;
        const int cn = mat->channels();
        const int w = mat->cols;
        const int h = mat->rows;

        std::vector<uint8_t> pixels(w * h * cn);
        for(int y = 0; y < h; y++)
        {
            for(int x = 0; x < w; x++)
            {
                const auto src = (h - 1 - y) * w * cn + x * cn;
                const auto dst = y * w * cn + x * cn;
                pixels[dst] = pixel_ptr[src];
            }
        }

        desc.format = ur2::TextureFormat::RED;
        m_tex = dev.CreateTexture(desc, pixels.data());
    }
        break;
    case CV_8UC3:
    {
        uint8_t* pixel_ptr = (uint8_t*)mat->data;
        const int cn = mat->channels();
        const int w = mat->cols;
        const int h = mat->rows;

        std::vector<uint8_t> pixels(w * h * cn);
        for(int y = 0; y < h; y++)
        {
            for(int x = 0; x < w; x++)
            {
                const auto src = (h - 1 - y) * w * cn + x * cn;
                const auto dst = y * w * cn + x * cn;
                pixels[dst + 0] = pixel_ptr[src + 2];
                pixels[dst + 1] = pixel_ptr[src + 1];
                pixels[dst + 2] = pixel_ptr[src + 0];
            }
        }

        desc.format = ur2::TextureFormat::RGB;
        m_tex = dev.CreateTexture(desc, pixels.data());
    }
        break;
    default:
        assert(0);
    }
}

void ImageRenderer::Draw(const ur2::Device& dev, ur2::Context& ctx) const
{
    if (!m_tex) {
        return;
    }

    auto rs = ur2::DefaultRenderState2D();
    pt2::RenderSystem::DrawTexture(dev, ctx, rs, m_tex, sm::rect(512, 512), sm::Matrix2D(), false);
}

void ImageRenderer::Clear()
{
    m_tex.reset();
}

}