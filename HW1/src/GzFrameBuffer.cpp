#include "GzFrameBuffer.h"

//Put your implementation here------------------------------------------------

void GzFrameBuffer::initFrameSize(GzInt width, GzInt height)
{
    width_ = width;
    height_ = height;
    colorBuffer.resize(width,vector<GzColor>(height));
    //cout<< colorBuffer.size() << endl;
    depthBuffer.resize(width,vector<GzReal>(height));
}

GzImage GzFrameBuffer::toImage()
{
    GzImage image(width_,height_);
    for(GzInt i = 0; i < width_; i++)
    {
        for(GzInt j = 0; j < height_; j++)
        {
            image.set(i,j,colorBuffer[i][j]);
        }
    }
    return image;
}

void GzFrameBuffer::clear(GzFunctional buffer)
{
    //clear color buffer with set clear color
    if(buffer & GZ_COLOR_BUFFER)
    {
        for(GzInt i = 0; i < width_; i++)
        {
            for(GzInt j = 0; j < height_; j++)
            {
                colorBuffer[i][j] = clearColor_;
            }
        }
    }

    //clear depth buffer with set default depth
    if(buffer & GZ_DEPTH_BUFFER)
    {
        for(GzInt i = 0; i < width_; i++)
        {
            for(GzInt j = 0; j < height_; j++)
            {
                depthBuffer[i][j] = clearDepth_;
            }
        }
    }
}

void GzFrameBuffer::setClearColor(const GzColor &color)
{
    clearColor_ = color;
}

void GzFrameBuffer::setClearDepth(GzReal depth)
{
    clearDepth_ = depth;
}

void GzFrameBuffer::drawPoint(const GzVertex &v, const GzColor &c, GzFunctional status)
{
    //clip the points outside the image
    if(v[0] > width_ || v[1] > height_ || v[0] < 0 || v[1] < 0)
    {   
        return;
    }
    //assign pixel value here
    if(status & GZ_DEPTH_TEST)
    {
        //depth test
        //draw the point with larger or equal z value
        if(v[2] >= depthBuffer[v[0]][v[1]])
        {
            colorBuffer[v[0]][v[1]] = c;
            //write the depth buffer
            depthBuffer[v[0]][v[1]] = v[2];
        }
    }
    else
    {
        //without depth test
        colorBuffer[v[0]][v[1]] = c;
    }
}
