#include "SpriteFont.h"

#include <fstream>

struct _FileCharDesc
{
	UINT32 id;
	UINT16 x;
	UINT16 y;
	UINT16 width;
	UINT16 height;
	INT16 xoffset;
	INT16 yoffset;
	INT16 xadvance;
	UINT8 page;
	UINT8 channel;
};

SpriteFont::SpriteFont()
{
	m_pTexture = nullptr;
	ZeroMemory(m_Chars, sizeof(m_Chars));
}

void SpriteFont::Load(char* p_pFilename, ID3D11Device* p_pDevice)
{
	std::ifstream _File(p_pFilename, std::ios::binary);

	if (_File.fail())
	{
		return;
	}

	int VersionID;
	_File.read((char*)&VersionID, 4);

	if ((VersionID & 0x00FFFFFF) != 0x00464d42)
	{
		return;
	}
	if ((VersionID & 0xFF000000) != 0x03000000)
	{
		return;
	}

	char _CurrentBlockType;
	int _CurrentBlockSize;
	char _Trash[4028];

	while (!_File.eof())
	{
		_File.read(&_CurrentBlockType, 1);
		_File.read((char*)&_CurrentBlockSize, 4);


		switch (_CurrentBlockType)
		{
		case 1: // Font info
			_File.read(_Trash, _CurrentBlockSize);
			break;

		case 2: // Common info
			_File.read(_Trash, _CurrentBlockSize);
			break;
		case 3: //
			char _Pages[1024];
			_File.read(_Pages, _CurrentBlockSize);
			m_pTexture = new Texture(p_pDevice, _Pages);
			break;
		case 4:
			int _CharCount;
			_CharCount = _CurrentBlockSize / 20;

			for (int x = 0; x < _CharCount; x++)
			{
				_FileCharDesc _Desc;
				_File.read((char*)&_Desc, 20);
				m_Chars[_Desc.id].m_X = _Desc.x;
				m_Chars[_Desc.id].m_Y = _Desc.y;
				m_Chars[_Desc.id].m_Width = _Desc.width;
				m_Chars[_Desc.id].m_Height = _Desc.height;

				m_Chars[_Desc.id].RelativeHeight = _Desc.height / 32.0f;
				m_Chars[_Desc.id].RelativeWidth = _Desc.width / 32.0f;
				m_Chars[_Desc.id].RelativeOffsetY = _Desc.yoffset / 32.0f;
			}
			break;
		case 5:
			_File.read(_Trash, _CurrentBlockSize);
			break;
		default:
			int asd = 0;
			break;
		}
	}
}