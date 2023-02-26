//#define BBOX
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
            image.set(i, j ,colorBuffer[i][j]);
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
    status_ = status;
    //clip the points outside the image
    if(v[0] >= width_ || v[1] >= height_ || v[0] < 0 || v[1] < 0)
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

void GzFrameBuffer::drawTriangle(const GzTriangle& tri, GzFunctional status)
{
    //transform from object coordinate to image coordinate
    GzTriangle tri_transed = tri;
    for(auto& vert : tri_transed.vertices)
    {
        vert.at(Y) = height_ - vert.at(Y) - 1;
    }
    status_ = status;
#ifdef BBOX
    drawTriangleUseBbox(tri_transed);
#else
    drawTriangleUseScanline(tri_transed);
#endif   
}

void GzFrameBuffer::drawPixel(const GzInt x, const GzInt y, const GzReal z, const GzColor &c)
{
    //assign pixel value here
    if(status_ & GZ_DEPTH_TEST)
    {
        //depth test
        //draw the point with larger or equal z value
        if(z >= depthBuffer[x][y])
        {
            colorBuffer[x][y] = c;
            //write the depth buffer
            depthBuffer[x][y] = z;
        }
    }
    else
    {
        //without depth test
        colorBuffer[x][y] = c;
    }
}

void GzFrameBuffer::drawTriangleUseBbox(const GzTriangle &tri)
{
    //calculate a bounding box for the triangle in the image to avoid testing every pixel
    Gz2DBoundingBox bbox = calculateBoundingBox(tri);
    for (int x = bbox.min[0]; x < bbox.max[0]; ++x)
    {
        for (int y = bbox.min[1]; y < bbox.max[1]; ++y)
        {
            GzReal px = x + 0.5;
            GzReal py = y + 0.5;
            bool inside = insideTriangle(tri,px,py);
            if(inside)
            {
                //Do interplotation for the triangle using barcentric coordinates.
                GzReal w1,w2,w3 = 0.0;
                barycentricInterpolation(px,py,tri, w1,w2,w3);

                GzColor c1 = tri.vertices[0].color;
                GzColor c2 = tri.vertices[1].color;
                GzColor c3 = tri.vertices[2].color;

                //intepolated z value
                GzReal z = tri.vertices[0][2]*w1 + tri.vertices[1][2]*w2 + tri.vertices[2][2]*w3;
                //intepolated color
                GzColor color = c1*w1 + c2*w2+ c3*w3;

                //draw the pixel
                drawPixel(x,y,z,color);
            }
        }

    }
}

void GzFrameBuffer::drawTriangleUseScanline(const GzTriangle &tri)
{
    vector<GzVertex> vertices = tri.vertices;

    //sort three vertices by y-coordinate ascending, so p1 is the topmost vertex
    sort(vertices.begin(),vertices.end(),
    [](const GzVertex &a, const GzVertex &b)->bool
    {
        return a.at(Y) < b.at(Y);
    }
    );
    
    GzVertex p1 = vertices[0];
    GzVertex p2 = vertices[1];
    GzVertex p3 = vertices[2];

    GzTriangle new_tri(p1,p2,p3);

    //bottom-flat triangle case
    if(GzInt(p2.at(Y)+0.5) == GzInt(p3.at(Y)+0.5))
    {
        fillBottomFlatTri(new_tri);
    }
    //top-flat triangle case
    else if (GzInt(p1.at(Y)+0.5) == GzInt(p2.at(Y)+0.5))
    {
        fillTopFlatTri(new_tri);
    }
    //general case - split the triangle to two triangles, one for bottom-flat and one for top-flat
    else
    {
        GzVertex p4;
        p4.at(Y) = p2.at(Y);

        // if((p3.at(X)-p1.at(X)) < 0)
        // {
        //     return;
        // }
        GzReal w = ((p4.at(Y)-p1.at(Y))/(p3.at(Y)-p1.at(Y)));

        // if((p3.at(X)-p1.at(X)) < 0)
        // {
        //     p4.at(X) = p1.at(X) - w *(p3.at(X)-p1.at(X));
        // }
        // else
        {
            p4.at(X) = p1.at(X) + w*(p3.at(X)-p1.at(X));
        }
       

        p4.at(X) = (GzInt)p4.at(X);

        //Do interplotation for the triangle using barcentric coordinates.
        GzReal w1,w2,w3 = 0.0;
        barycentricInterpolation(p4.at(X),p4.at(Y),tri,w1,w2,w3);

        GzColor c1 = tri.vertices[0].color;
        GzColor c2 = tri.vertices[1].color;
        GzColor c3 = tri.vertices[2].color;

        //intepolated z value
        GzReal z = tri.vertices[0][2]*w1 + tri.vertices[1][2]*w2 + tri.vertices[2][2]*w3;
        //intepolated color
        GzColor color = c1*w1 + c2*w2+ c3*w3;
        p4.at(Z) = z;
        p4.color = color;

        //bottom triangle
        GzTriangle bottom(p1,p2,p4);
        fillBottomFlatTri(bottom);
        //top triangle
        GzTriangle top(p2,p4,p3);
        fillTopFlatTri(top);
    }
}

void GzFrameBuffer::fillBottomFlatTri(const GzTriangle &tri)
{
    GzVertex p1 = tri.vertices[0];
    GzVertex p2 = tri.vertices[1];
    GzVertex p3 = tri.vertices[2];

    GzColor c1 = tri.vertices[0].color;
    GzColor c2 = tri.vertices[1].color;
    GzColor c3 = tri.vertices[2].color;
    
    //p2p1
    GzReal invslope1 = (p2.at(X) - p1.at(X))/(p2.at(Y) - p1.at(Y));
    //p3p1
    GzReal invslope2 = (p3.at(X) - p1.at(X))/(p3.at(Y) - p1.at(Y));

    GzReal x1 = p1.at(X);
    GzReal x2 = p1.at(X);

    //scan line by line, draw a straight horizontal line between x1 and x2
    for(GzInt scanY = p1.at(Y); scanY <= p2.at(Y); )
    {
        GzVertex v1;
        v1.at(X) = x1;
        v1.at(Y) = scanY;
        //Do interplotation for the triangle using barcentric coordinates.
        GzReal w1,w2,w3 = 0.0;
        barycentricInterpolation(v1.at(X),v1.at(Y),tri, w1,w2,w3);
        //intepolated z value
        GzReal z = tri.vertices[0][2]*w1 + tri.vertices[1][2]*w2 + tri.vertices[2][2]*w3;
        //intepolated color
        GzColor color = c1*w1 + c2*w2+ c3*w3;
        v1.at(Z) = z;
        v1.color = color;

        GzVertex v2;
        v2.at(X) = x2;
        v2.at(Y) = scanY;
        //Do interplotation for the triangle using barcentric coordinates.
        barycentricInterpolation(v2.at(X),v2.at(Y),tri, w1,w2,w3);
        //intepolated z value
        z = tri.vertices[0][2]*w1 + tri.vertices[1][2]*w2 + tri.vertices[2][2]*w3;
        //intepolated color
        color = c1*w1 + c2*w2+ c3*w3;
        v2.at(Z) = z;
        v2.color = color;

        //draw horizontal line from (x1,scanY) to (x2,scanY)
        drawLine(v1,v2,status_);
        //update the two endpoints
        x1 += invslope1;
        if(x1 )
        x2 += invslope2;
        scanY += 1;
    }
}

void GzFrameBuffer::fillTopFlatTri(const GzTriangle &tri)
{

    GzVertex p1 = tri.vertices[0];
    GzVertex p2 = tri.vertices[1];
    GzVertex p3 = tri.vertices[2];

    GzColor c1 = tri.vertices[0].color;
    GzColor c2 = tri.vertices[1].color;
    GzColor c3 = tri.vertices[2].color;

    //p1p3
    GzReal invslope1 = (p3.at(X) - p1.at(X))/(p3.at(Y) - p1.at(Y));
    //p2p3
    GzReal invslope2 = (p3.at(X) - p2.at(X))/(p3.at(Y) - p2.at(Y));
    
    GzReal x1 = p3.at(X);
    GzReal x2 = p3.at(X);

    //scan line by line, draw a straight horizontal line between x1 and x2
    for(GzInt scanY = p3.at(Y); scanY > p1.at(Y); scanY--)
    {
        GzVertex v1;
        v1.at(X) = x1;
        v1.at(Y) = scanY;
        //Do interplotation for the triangle using barcentric coordinates.
        GzReal w1,w2,w3 = 0.0;
        barycentricInterpolation(v1.at(X),v1.at(Y),tri, w1,w2,w3);
        //intepolated z value
        GzReal z = tri.vertices[0][2]*w1 + tri.vertices[1][2]*w2 + tri.vertices[2][2]*w3;
        //intepolated color
        GzColor color = c1*w1 + c2*w2+ c3*w3;
        v1.at(Z) = z;
        v1.color = color;

        GzVertex v2;
        v2.at(X) = x2;
        v2.at(Y) = scanY;
        //Do interplotation for the triangle using barcentric coordinates.
        barycentricInterpolation(v2.at(X),v2.at(Y),tri, w1,w2,w3);
        //intepolated z value
        z = tri.vertices[0][2]*w1 + tri.vertices[1][2]*w2 + tri.vertices[2][2]*w3;
        //intepolated color
        color = c1*w1 + c2*w2+ c3*w3;
        v2.at(Z) = z;
        v2.color = color;

        //draw horizontal line from (x1,scanY) to (x2,scanY)
        drawLine(v1,v2,status_);
        //update the two endpoints
        x1 -= invslope1;
        x2 -= invslope2;
    }
}

void GzFrameBuffer::drawLine(const GzVertex& v1, const GzVertex& v2, GzFunctional status)
{
    //drawPoint(v1,v1.color,status);
    //drawPoint(v2,v2.color,status);

    if(v1.at(X) > v2.at(X))
    {
        drawLine(v2,v1,status);
        return;
    }
    GzVertex curr = v1;
    GzColor c1 = v1.color;
    GzColor c2 = v2.color;
    while((GzInt)curr.at(X) <= (GzInt)v2.at(X))
    {
        GzReal w = (curr.at(X) - v1.at(X))/ (v2.at(X) - v1.at(X));
        curr.at(Z) = v2.at(Z) * w +  v1.at(Z) * (1.0-w);
        
        curr.color = c2 * w + c1 * (1.0-w);
        drawPoint(curr,curr.color,status);
        curr.at(X) += 1.0;
    }
}

bool GzFrameBuffer::insideTriangle(const GzTriangle &tri, const GzReal x, const GzReal y)
{
    //calculate the dot product sign for p1p3 and p1p2
    auto sign =[](GzVertex p1, GzVertex p2, GzVertex p3) 
    {
         return (p1[0] - p3[0]) * (p2[1] - p3[1]) - (p2[0] - p3[0]) * (p1[1] - p3[1]); 
    };
    float d1, d2, d3;
    bool has_neg, has_pos;

    GzVertex pt(x,y,0.);

    d1 = sign(pt, tri.vertices[0], tri.vertices[1]);
    d2 = sign(pt, tri.vertices[1], tri.vertices[2]);
    d3 = sign(pt, tri.vertices[2], tri.vertices[0]);

    has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

    return !(has_neg && has_pos);
}

void GzFrameBuffer::barycentricInterpolation(const GzReal x, const GzReal y, const GzTriangle &tri, GzReal &w1, GzReal &w2, GzReal &w3)
{
    GzVertex p1 = tri.vertices[0];
    GzVertex p2 = tri.vertices[1];
    GzVertex p3 = tri.vertices[2];
                
    GzReal denominator = (p2[1] - p3[1])*(p1[0] - p3[0]) + (p3[0]-p2[0])*(p1[1]-p3[1]);
    //the weight to p1
    w1 = (p2[1] - p3[1])*(x - p3[0]) + (p3[0]-p2[0])*(y-p3[1]);
    w1 /= denominator;
    //the weight to p2
    w2 = (p3[1] - p1[1])*(x - p3[0]) + (p1[0]-p3[0])*(y-p3[1]);
    w2 /= denominator;
    //the weight to p3
    w3 = 1.0 - w1 - w2;
}

Gz2DBoundingBox GzFrameBuffer::calculateBoundingBox(const GzTriangle &tri)
{
    //find minimal x,y for the three vertices
    GzInt x = min(tri.vertices[0][0],tri.vertices[1][0]);
    x = min(x,GzInt(tri.vertices[2][0]));
    x = max(x, 0);
    GzInt y = min(tri.vertices[0][1],tri.vertices[1][1]);
    y = min(y,GzInt(tri.vertices[2][1]));
    y = max(y, 0);

    //find miximum x,y for the tree vertices, we need to round up the double
    GzInt x_max = max(tri.vertices[0][0]+0.5,tri.vertices[1][0]+0.5);
    x_max = max(x_max,GzInt(tri.vertices[2][0]+0.5));
    x_max = min(x_max, width_);
    GzInt y_max = max(tri.vertices[0][1]+0.5,tri.vertices[1][1]+0.5);
    y_max = max(y_max,GzInt(tri.vertices[2][1]+0.5));
    y_max = min(y_max, height_);

    Gz2DBoundingBox bbox;
    bbox.min = GzVertex(x,y,0);
    bbox.max = GzVertex(x_max,y_max,0);
    return bbox;
}
