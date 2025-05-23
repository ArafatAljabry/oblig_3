#ifndef VISUALOBJECT_H
#define VISUALOBJECT_H

#include <QVulkanWindow>
#include <vector>
#include "Utilities.h"


class VisualObject
{
public:
    VisualObject();

    void move(float x, float y = 0.0f, float z = 0.0f);
    void scale(float s);
    void rotate(float t, float x, float y, float z);

	//Setters and Getters
    inline std::vector<Vertex> getVertices() { return mVertices; }
    inline VkBuffer& getVBuffer() { return mVertexBuffer.mBuffer; }
    inline VkDeviceMemory& getVBufferMemory() { return mVertexBuffer.mBufferMemory; }
	inline VkDeviceMemory& getIBufferMemory() { return mIndexBuffer.mBufferMemory; }
    inline void setVBuffer(VkBuffer bufferIn) { mVertexBuffer.mBuffer = bufferIn; }
    inline void setVBufferMemory(VkDeviceMemory bufferMemoryIn) { mVertexBuffer.mBufferMemory = bufferMemoryIn; }
    inline VkBuffer& getIBuffer() { return mIndexBuffer.mBuffer; }
    inline void setIBuffer(VkBuffer bufferIn) { mIndexBuffer.mBuffer = bufferIn; }
    inline void setIBufferMemory(VkDeviceMemory bufferMemoryIn) { mIndexBuffer.mBufferMemory = bufferMemoryIn; }
    inline void setName(std::string name) { mName = name; }
    inline std::string getName() const { return mName; }
    inline int getDrawType() const { return drawType; }
    inline QMatrix4x4 getMatrix() const {return mMatrix;}
	inline std::vector<Vertex> getVertices() const { return mVertices; }
	inline std::vector<uint32_t> getIndices() const { return mIndices; }

    TextureHandle mTexturehandle;
    //for collision
    float radius{0.5f};

    void setPosition(const QVector3D& pos);
    QVector3D getPosition();
    std::string getTag() const{return mTag;}
    void setTag(std::string tag);

    bool enableCollision{true}; //Won't trigger collision logic if false

    //for the door,would be nice on the wall class, to be continued
    bool isOpen{false};
protected:
    std::vector<Vertex> mVertices;
    std::vector<uint32_t> mIndices;
    QMatrix4x4 mMatrix;
    std::string mName;
    std::string mTag{"actor"};
	BufferHandle mVertexBuffer;
	BufferHandle mIndexBuffer;
    //VkPrimitiveTopology mTopology{ VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST }; //not used

    int drawType{ 0 }; // 0 = fill, 1 = line
};

#endif // VISUALOBJECT_H

