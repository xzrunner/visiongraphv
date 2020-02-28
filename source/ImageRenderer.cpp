#include "visiongraphv/ImageRenderer.h"

#include <unirender/Blackboard.h>
#include <unirender/RenderContext.h>
#include <painting2/RenderSystem.h>
#include <renderpipeline/RenderMgr.h>

#include <opencv2/opencv.hpp>

namespace vgv
{

void ImageRenderer::Setup(const std::shared_ptr<cv::Mat>& mat)
{
    if (!mat || mat->empty()) {
        return;
    }

    auto t = mat->type();

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

        auto& rc = ur::Blackboard::Instance()->GetRenderContext();
        int tex_id = rc.CreateTexture(pixels.data(), w, h, ur::TEXTURE_RED);
        m_tex = std::make_unique<ur::Texture>(&rc, w, h, ur::TEXTURE_RED, tex_id);
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

        auto& rc = ur::Blackboard::Instance()->GetRenderContext();
        int tex_id = rc.CreateTexture(pixels.data(), w, h, ur::TEXTURE_RGB);
        m_tex = std::make_unique<ur::Texture>(&rc, w, h, ur::TEXTURE_RGB, tex_id);
    }
        break;
    default:
        assert(0);
    }
}

void ImageRenderer::Draw() const
{
    if (!m_tex) {
        return;
    }

    auto& rc = ur::Blackboard::Instance()->GetRenderContext();
    rc.SetZTest(ur::DEPTH_DISABLE);

    pt2::RenderSystem::DrawTexture(*m_tex, sm::rect(512, 512), sm::Matrix2D(), false);
}

void ImageRenderer::Clear()
{
    m_tex.reset();
}

}