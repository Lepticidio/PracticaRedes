#include "GameBuffer.h"

CGameBuffer::CGameBuffer(size_t initsize, size_t delta)
{

}

CGameBuffer::~CGameBuffer()
{
}

void CGameBuffer::write(const FVector& _data)
{
  write(_data.X);
  write(_data.Y);
  write(_data.Z);
}

void CGameBuffer::read(FVector& data_)
{
  read(data_.X);
  read(data_.Y);
  read(data_.Z);
}

void CGameBuffer::write(const FVector2D& _data)
{
  write(_data.X);
  write(_data.Y);
}

void CGameBuffer::read(FVector2D& data_)
{
  read(data_.X);
  read(data_.Y);
}

void CGameBuffer::write(Net::NetMessageType _data)
{
  write(&_data, sizeof(_data));
}

void CGameBuffer::read(Net::NetMessageType& data_)
{
  read(&data_, sizeof(data_));
}
