#ifndef ProjectH_ImgResource_H
#define ProjectH_ImgResource_H

#include <iostream>
#include <vector>
#include <string>
#include <hash_map>
#include <fstream>
#include <algorithm>

namespace ProjectH
{
	class ImgResource
	{
		friend class DistinctionBtn;

	public:
		struct ImageData
		{
			int x, y;			/* 시작점 x, y */	
			int width;			/* x + width */
			int height;			/* y + height */
			int centerPointX;	/* x + x 중심값 */
			int centerPointY;	/* y + y 중심값 */
			int availableTile;	/* 해당 이미지가 어느 타일위에 올려질 수 있는지 판별. -1이면 모든타일에 올라갈 수 있음 */
			int type;			/* 이미지의 타입 */
			int isBtn;			/* 버튼 여부 */
			int tileType;		/* 타일의 종류(키 값) */
			int shadow;			/* 그림자의 종류 */
			bool moveable;		/* 이동 가능여부 */
		};

		typedef enum
		{
			MAJOR_TILE = 0,		/* 대표타일 */
			AROUND_TILE,		/* 주변타일 */
			GENERAL_TILE,		/* 일반타일 */
			ACCESSORY,			/* 액세서리 */
			PILLAR				/* 기둥 */
		} ImageType;

		typedef enum
		{
			GRASS		= 0,
			STON,
			CASTLE
		} Theme;

	public:
		static
		ImgResource*	Instance		();

		~ImgResource() { logFile.close(); }

		int ImageIndex(const int, const int);									/* 이미지 인덱스 리턴 */
		int TileWidth() const { return info[static_cast<int>(theme)].width; }	/* 타일의 가로길이 리턴 */
		int TileHeight() const { return info[static_cast<int>(theme)].height; }	/* 타일의 세로길이 리턴 */
		bool DoodadAvailable(const int idx, const int imgIdx);					/* Doodad의 조합가능 여부 리턴 */

		/* 사용자가 넘겨준 키 값을 이용하여 이미지의 실제 데이터들을
		 * 사용자가 넘겨준 ImageData 구조체에 리턴하여 준다.
		 * 만약 키 값에 해당하는 데이터를 찾을 수 없을경우, false를
		 * 리턴하여 해당값을 얻는데 실패했음을 알림.
		 */
		bool GetImageData(const int, const int, ImageData&);
		bool GetImageData(const int, ImageData&);
		const std::string& GetBtnImgPath() const { return imgPaths[static_cast<int>(theme)]; }
		const std::vector<std::string>& GetThemeList() { return themeList; }
		const int NumberOfComboBox() const { return nComboBox; }
		const std::string& ComboBoxName(int idx) const { return ComboNames[idx]; }
		const int GetTileType(const int imgIdx) { return imgDataColl[theme][imgIdx].tileType; }

		/* 타일과 연결되는 부분의 기둥값에 대한 채움 */
		void GetPillar(const int tileType, ImageData &container, bool top = false);

		/* Doodad 인덱스, TileType을 받아서 그림자 인덱스 리턴 */
		int GetShadowIndex(const int tileType, const int doodadIdx);

		/* 사용자에게 버튼이미지에 관련된 이미지 정보만을 구별해서
		 * 넘겨준다.
		 */
		void GetBtnImgData(std::vector<ImageData>&, std::vector<int>&);

		/* 테마값을 설정 */
		void SetTheme(int thm = 0) { theme = static_cast<Theme>(thm); }

		/* 사용자가 넘겨준 인덱스에 해당하는 Doodad 데이터를 채워줌. */
		bool GetDoodadData(const int, ImageData&);

		/* 사용자가 넘겨준 이미지 인덱스 값이 Doodad값인지 여부를 리턴 */
		bool IsDoodad(const int);

		/* 사용자가 넘겨준 타일 및 이미지 인덱스가 조합이 가능한지
		 * 여부를 리턴한다.
		 */
		bool AvailableBrush(const int, const int);

		/* 대표타일 여부 */
		bool IsMajorTile(const int imgIdx) { return CheckImageType(imgIdx, MAJOR_TILE); }

		/* 주변타일 여부 */
		bool IsAroundTile(const int imgIdx) { return CheckImageType(imgIdx, AROUND_TILE); }
		
		/* 일반타일 여부 */
		bool IsGeneralTile(const int imgIdx) { return CheckImageType(imgIdx, GENERAL_TILE); }

		/* 액세서리 여부 */
		bool IsAccessory(const int imgIdx) { return CheckImageType(imgIdx, ACCESSORY); }

		/* 기둥 여부 */
		bool IsPillar(const int imgIdx) { return CheckImageType(imgIdx, PILLAR); }

		/* 이동 여부 */
		bool IsMoveable(const int imgIdx) { return imgDataColl[theme][imgIdx].moveable; }

	private:
		ImgResource(std::string fpath = "Data/ImgResource.txt", Theme theme = GRASS);

		void LoadResource();
		std::ifstream& LoadVersionNum(std::ifstream&, int);
		std::ifstream& LoadTileWidth(std::ifstream&, int);
		std::ifstream& LoadTileHeight(std::ifstream&, int);
		std::ifstream& LoadImageData(std::ifstream&, int);
		std::ifstream& LoadImageIndex(std::ifstream&, int);
		std::ifstream& LoadDoodadInfo(std::ifstream&, int);
		std::ifstream& LoadNumOfCombo(std::ifstream&);
		std::ifstream& LoadComboNames(std::ifstream&);

		/* 테마 값을 읽어들임 */
		std::ifstream& ReadTheme(std::ifstream&, Theme&);

		/* 이 메소드는 한 라인에 int형 값이 하나일 경우에만 쓰여야 하며,
		 * 한 라인을 읽어들인후 해당 값을 int형으로 변환하여 저장하여 준다.
		 */
		std::ifstream& GetOneValueInLine(std::ifstream&, int&, std::string);
		
		/* 탭을 구분자로 하여 문자열을 쪼갠다. */
		void SplitString(std::string&, std::vector<std::string>&);

		bool CheckImageType(int, ImageType);

	protected:
		struct GeneralInfo 
		{
			int	version;			/* 버젼 넘버 */
			int	width;				/* 타일의 가로 길이 */
			int	height;				/* 타일의 세로 길이 */
			int	nImgData;			/* Number of ImageData */
			int	nDoodad;			/* Number of DoodadData */
			int doodadStartNum;		/* Doodad 시작 번호 */
		};

	protected:
		Theme theme;							/* 테마 값 */
		std::string fileListPath;				/* 이미지 리소스 파일들의 리스트가 들어있는 파일의 위치 */
		std::ofstream logFile;					/* Log를 기록하기 위한 변수 */
		std::vector<std::string> imgPaths;		/* 버튼에 사용될 이미지의 테마별 경로들 */	
		std::vector<std::string> themeList;		/* 테마 문자열 리스트 */
		std::vector<std::string> ComboNames;	/* 콤보박스에 들어갈 이름들 */
		int nComboBox;							/* 콤보박스 종류의 개수 */

		/* 테마별로 버젼, 타일의 가로 및 세로, 이미지 데이터의 갯수,
		 * Doodad의 갯수, Doodad 시작번호 등을 저장하고 있는 벡터
		 */
		std::vector<GeneralInfo> info;

		/* 이미지 데이터 인덱스를 저장하고 있는 해쉬맵 */
		std::vector<std::vector<int> > imgIdxColl;

		/* Doodad 데이터를 저장하고 있는 해쉬맵 */
		std::vector<std::vector<int> > DoodadDataColl;

		/* 이미지 데이터를 저장하고 있는 벡터 */
		std::vector<std::vector<ImageData> > imgDataColl;
	};


	/* 이미지 데이터에서 버튼이미지를 구분하기 위한
	 * Function Object
	 */
	class DistinctionBtn
	{
	public:
		bool operator() (const ImgResource::ImageData &val)
		{
			if (val.isBtn)
				return true;
			return false;
		}
	};

	class ValueGenerator
	{
	public:
		ValueGenerator(const int val): value(val) {}
		void operator() (int &val)
		{
			val = value;
		}

	private:
		int value;
	};
} /* namespace ProjectH */

#define IMGRESOURCE() ProjectH::ImgResource::Instance()

#endif /* ProjectH_ImgResource_H */