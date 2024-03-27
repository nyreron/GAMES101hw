//
// Created by LEI XU on 4/27/19.
//

#ifndef RASTERIZER_TEXTURE_H
#define RASTERIZER_TEXTURE_H
#include "global.hpp"
#include <Eigen/Eigen>
#include <opencv2/opencv.hpp>
class Texture{
private:
    cv::Mat image_data;

public:
    Texture(const std::string& name)
    {
        image_data = cv::imread(name);
        cv::cvtColor(image_data, image_data, cv::COLOR_RGB2BGR);
        width = image_data.cols;
        height = image_data.rows;
    }

    int width, height;

    Eigen::Vector3f getColor(float u, float v)
    {
        if(u<0) u=0;
        if(v<0) v=0;
        if(u>1) u=1;
        if(v>1) v=1;
        
        auto u_img = u * width;
        auto v_img = (1 - v) * height;
        auto color = image_data.at<cv::Vec3b>(v_img, u_img);
        return Eigen::Vector3f(color[0], color[1], color[2]);
    }

    cv::Vec3b lerp(float ratio, cv::Vec3b color1, cv::Vec3b color2)
    {
        return cv::Vec3b((1.0f-ratio)*color1[0] + ratio*1.0f*color2[0], (1.0f-ratio)*color1[1] + ratio*1.0f*color2[1], (1.0f-ratio)*color1[2] + ratio*1.0f*color2[2]);
    }

    Eigen::Vector3f getBilinearInterpolationColor(float u, float v)
    {
        auto down_width = width;
        auto down_height = height;
        float u_down_img = std::min((down_width - 1) * 1.0f, std::max(0.0f, u * down_width));
        float v_down_img = std::min((down_height - 1) * 1.0f, std::max(0.0f, (1 - v) * down_height));
        cv::Vec3b bilinear_interpolate_color;

        float u_down_img_round = std::min((down_width - 1) * 1.0f, std::max(0.0f, round(u_down_img)));
        float v_down_img_round = std::min((down_height - 1) * 1.0f, std::max(0.0f, round(v_down_img)));

        if (u_down_img - u_down_img_round >= 0.0f)
        {
            if (v_down_img - v_down_img_round >= 0.0f) // near to left_top
            {
                float s = u_down_img - u_down_img_round + 0.5f;
                float t = v_down_img - v_down_img_round + 0.5f;
                int x2 = std::max(0, std::min(down_width - 1, (int)u_down_img_round));
                int x1 = std::max(0, std::min(down_width - 1, x2 - 1));
                int y2 = std::max(0, std::min(down_height - 1, (int)v_down_img_round));
                int y1 = std::max(0, std::min(down_height - 1, y2 - 1));
                bilinear_interpolate_color = lerp(t, lerp(s, image_data.at<cv::Vec3b>(y1, x1), image_data.at<cv::Vec3b>(y1, x2)),
                                                  lerp(s, image_data.at<cv::Vec3b>(y2, x1), image_data.at<cv::Vec3b>(y2, x2)));
            }
            else
            { // near to left_bottom
                float s = u_down_img - u_down_img_round + 0.5f;
                float t = v_down_img_round - v_down_img + 0.5f;
                int x2 = std::max(0, std::min(down_width - 1, (int)u_down_img_round));
                int x1 = std::max(0, std::min(down_width - 1, x2 - 1));
                int y2 = std::max(0, std::min(down_height - 1, (int)v_down_img_round));
                int y1 = std::max(0, std::min(down_height - 1, y2 - 1));
                bilinear_interpolate_color = lerp(t, lerp(s, image_data.at<cv::Vec3b>(y2, x1), image_data.at<cv::Vec3b>(y2, x2)),
                                                  lerp(s, image_data.at<cv::Vec3b>(y1, x1), image_data.at<cv::Vec3b>(y1, x2)));
            }
        }
        else
        {
            if (v_down_img - v_down_img_round >= 0.0f) // near to right_top
            {
                float s = u_down_img_round - u_down_img + 0.5f;
                float t = v_down_img - v_down_img_round + 0.5f;
                int x2 = std::max(0, std::min(down_width - 1, (int)u_down_img_round));
                int x1 = std::max(0, std::min(down_width - 1, x2 - 1));
                int y2 = std::max(0, std::min(down_height - 1, (int)v_down_img_round));
                int y1 = std::max(0, std::min(down_height - 1, y2 - 1));
                bilinear_interpolate_color = lerp(t, lerp(s, image_data.at<cv::Vec3b>(y1, x2), image_data.at<cv::Vec3b>(y1, x1)),
                                                  lerp(s, image_data.at<cv::Vec3b>(y2, x2), image_data.at<cv::Vec3b>(y2, x1)));
            }
            else
            { // near to right_bottom
                float s = u_down_img_round - u_down_img + 0.5f;
                float t = v_down_img_round - v_down_img + 0.5f;
                int x2 = std::max(0, std::min(down_width - 1, (int)u_down_img_round));
                int x1 = std::max(0, std::min(down_width - 1, x2 - 1));
                int y2 = std::max(0, std::min(down_height - 1, (int)v_down_img_round));
                int y1 = std::max(0, std::min(down_height - 1, y2 - 1));
                bilinear_interpolate_color = lerp(t, lerp(s, image_data.at<cv::Vec3b>(y2, x2), image_data.at<cv::Vec3b>(y2, x1)),
                                                  lerp(s, image_data.at<cv::Vec3b>(y1, x2), image_data.at<cv::Vec3b>(y1, x1)));
            }
        }

        return Eigen::Vector3f(bilinear_interpolate_color[0], bilinear_interpolate_color[1], bilinear_interpolate_color[2]);
    }

    Eigen::Vector3f getColorBilinear(float u, float v)
    {
        u = std::clamp(u, 0.00001f, 0.9999f);
        v = std::clamp(v, 0.00001f, 0.9999f);

        int uMin, vMin, uMax, vMax;
        float u_img = u * width;
        float v_img = (1 - v) * height;
        int ujud = floor(u_img), vjud = floor(v_img);
        if ((u_img - ujud) <= 0.5f)
        {
            uMin = ujud - 1;
            uMax = ceil(u_img);
        }
        else
        {
            uMin = ujud;
            uMax = ceil(u_img) + 1;
        }
        if ((v_img - vjud) <= 0.5f)
        {
            vMin = vjud - 1;
            vMax = ceil(v_img);
        }
        else
        {
            vMin = vjud;
            vMax = ceil(v_img);
        }

        uMax = std::clamp(uMax, 0, width);
        uMin = std::clamp(uMin, 0, width);
        vMax = std::clamp(vMax, 0, height);
        vMin = std::clamp(vMin, 0, height);

        auto p1 = image_data.at<cv::Vec3b>(vMax, uMin);
        auto p2 = image_data.at<cv::Vec3b>(vMax, uMax);
        auto p3 = image_data.at<cv::Vec3b>(vMin, uMin);
        auto p4 = image_data.at<cv::Vec3b>(vMin, uMax);

        auto c1 = p1 + (u_img - uMin) / (uMax - uMin) * (p2 - p1);
        auto c2 = p3 + (u_img - uMin) / (uMax - uMin) * (p4 - p3);
        auto color = c2 + (c1 - c2) * (v_img - vMin) / (vMax - vMin);

        return Eigen::Vector3f(color[0], color[1], color[2]);
    }
};
#endif //RASTERIZER_TEXTURE_H
