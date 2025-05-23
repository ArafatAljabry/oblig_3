#include "HeightMap.h"
#include "Vertex.h"
#include "stb_image.h"

HeightMap::HeightMap()
{ }

void HeightMap::makeTerrain(std::string heightMapImage)
{
	//Load the heightmap image
	//Using stb_image to load the image
	stbi_uc* pixelData = stbi_load(heightMapImage.c_str(), &mWidth, &mHeight, &mChannels, STBI_rgb_alpha);
	if (pixelData == nullptr)
    {
	    qDebug() << "Failed to load heightmap image!";
	    return;
	}
	//Make the terrain from the pixel data
	makeTerrain(pixelData, mWidth, mHeight);
	stbi_image_free(pixelData);

}

//Function that makes a terrain grid from a heightmap, using the values in the heightmap as height.
//This function will crash if the width and height of the heightmap is not set correct!
//The size of the textureData array is widthIn * heightIn.
// The function is not tested in this codebase, and is provided as an example.
void HeightMap::makeTerrain(unsigned char* textureData, int widthIn, int heightIn)
{
    //Default normal pointing straight up - should be calculated correctly for lights to work!!!
    float normal[3]{0.f, 1.f, 0.f};

    //How many meters between each vertex in both x and z direction
    //This should be sent in as a parameter!
    float horisontalSpacing{.2f};

    //Scaling the height read from the heightmap. 0 -> 255 meters if this is set to 1
    //This should be sent in as a parameter!
    float heightSpacing{.02f};

    //Offset the whole terrain in y (height) axis
    //Moves the terrain mesh up or down
    //Because of Barycentric calculations, we want the terrain to be in World coordinates!
	//So we don't want to move the terrain up or down in the Y axis after it is made
    float heightPlacement{-10.f};

    //Getting the scale of the heightmap
    //Using depth as the name of texture height, to not confuse with terrain height
    unsigned short width = widthIn;       //Width == x-axis
    unsigned short depth = heightIn;      //Depth == z-axis

    //Temp variables for creating the mesh
    //Adding offset so the middle of the terrain will be in World origo
    float vertexXStart{ 0.f - width * horisontalSpacing / 2 };            // if world origo should be at center use: {0.f - width * horisontalSpacing / 2};
    float vertexZStart{ 0.f + depth * horisontalSpacing / 2 };            // if world origo should be at center use: {0.f + depth * horisontalSpacing / 2};

    //Loop to make the mesh from the values read from the heightmap (textureData)
	//Double for-loop to make the depth and the width of the terrain in one go
    for(int d{0}; d < depth; ++d)       //depth loop
    {
        for(int w{0}; w < width; ++w)   //width loop
        {
            //Heightmap image is actually stored as an one dimentional array - so calculating the correct index for column and row
            //and scale it according to variables
            // Calculate the correct index for the R value of each pixel
            int index = (w + d * width) * 4; // Each pixel has 4 bytes (RGBA)
            float heightFromBitmap = static_cast<float>(textureData[index]) * heightSpacing + heightPlacement;
			//                                          x - value                      y-value               z-value
            mVertices.emplace_back(Vertex{vertexXStart + (w * horisontalSpacing), heightFromBitmap, vertexZStart - (d * horisontalSpacing),
				//  dummy normal=0,1,0                  Texture coordinates
                normal[0],normal[1],normal[2],           w / (width - 1.f), d / (depth - 1.f)});
        }
    }

    // The mesh(grid) is drawn in quads with diagonals from lower left to upper right
    //          _ _
    //         |/|/|
    //          - -
    //         |/|/|
    //          - -
    //Making the indices for this mesh:
    for(int d{0}; d < depth-1; ++d)        //depth - 1 because we draw the last quad from depth - 1 and in negative z direction
    {
        for(int w{0}; w < width-1; ++w)    //width - 1 because we draw the last quad from width - 1 and in positive x direction
        {
			//Indices for one quad:
            mIndices.emplace_back(w + d * width);               // 0 + 0 * mWidth               = 0
            mIndices.emplace_back(w + d * width + width + 1);   // 0 + 0 * mWidth + mWidth + 1  = mWidth + 1
            mIndices.emplace_back(w + d * width + width);       // 0 + 0 * mWidth + mWidth      = mWidth
            mIndices.emplace_back(w + d * width);               // 0 + 0 * mWidth               = 0
            mIndices.emplace_back(w + d * width + 1);           // 0 + 0 * mWidth + 1           = 1
            mIndices.emplace_back(w + d * width + width + 1);   // 0 + 0 * mWidth + mWidth + 1  = mWidth + 1
        }
    }

 
	//Calculating the normals for the mesh
    //Function not made yet:
    //calculateHeighMapNormals();
}

float HeightMap::getHeightAt(const QVector3D& positionXZ) const
{
    QVector2D point(positionXZ.x(), positionXZ.z());

    for (size_t i = 0; i < mIndices.size(); i += 3) {
        const Vertex& v0 = mVertices[mIndices[i]];
        const Vertex& v1 = mVertices[mIndices[i + 1]];
        const Vertex& v2 = mVertices[mIndices[i + 2]];

        QVector2D a(v0.x, v0.z);
        QVector2D b(v1.x, v1.z);
        QVector2D c(v2.x, v2.z);

        QVector2D v0v2 = c - a;
        QVector2D v0v1 = b - a;
        QVector2D v0p = point - a;

        float d00 = QVector2D::dotProduct(v0v1, v0v1);
        float d01 = QVector2D::dotProduct(v0v1, v0v2);
        float d11 = QVector2D::dotProduct(v0v2, v0v2);
        float d20 = QVector2D::dotProduct(v0p, v0v1);
        float d21 = QVector2D::dotProduct(v0p, v0v2);

        float denom = d00 * d11 - d01 * d01;
        if (denom == 0.0f) continue;

        float v = (d11 * d20 - d01 * d21) / denom;
        float w = (d00 * d21 - d01 * d20) / denom;
        float u = 1.0f - v - w;

        if (u >= 0.0f && v >= 0.0f && w >= 0.0f) {
            return v0.y * u + v1.y * v + v2.y * w;
        }
    }

    return 0.0f;  // Default flat
}

