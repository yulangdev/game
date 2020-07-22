#include "stdafx.h"
#include "ImgResource.h"
#include <stdexcept>
#include <utility>

using std::ifstream;
using std::ofstream;
using std::vector;
using stdext::hash_map;
using std::getline;
using std::string;
using std::logic_error;	
using std::make_pair;
using stdext::hash_map;

namespace ProjectH
{
	ImgResource::ImgResource(string fpath, Theme thm)
	{
		logFile.open("Log/Resource.log", ofstream::app);
		fileListPath = fpath;
		SetTheme(thm);
		LoadResource();
	}

	/* 테마 값을 설정한다. */
	void ImgResource::LoadResource()
	{
		ifstream fileList;
		ifstream inputFile;

		/* 파일 리스트가 저장되어있는 파일을 오픈 */
		fileList.open(fileListPath.c_str(), ifstream::in);
		if (fileList.bad())
		{
			logFile.write("SetTheme(): 파일리스트 정보파일 오픈 실패\n",
						  strlen("SetTheme(): 파일리스트 정보파일 오픈 실패\n"));
		}
		else
		{
			string file;
			int nLine;
			
			/* 파일리스트 파일의 라인수를 체크 */
			for (nLine = 0; fileList.eof() == false; ++nLine)
				getline(fileList, file);
			file.clear();
			
			/* 파일의 Seek를 다시 처음으로 되돌림 */
			fileList.seekg(0, std::ios::beg);
			
			imgPaths.resize(5);
			Theme thm;
			while (fileList.good() && fileList.eof() == false)
			{
				/* 테마 값을 읽어들임 */
				ReadTheme(fileList, thm);

				getline(fileList, file);
				imgPaths[thm] = file;

				getline(fileList, file);

				/* 리소스 정보 파일 오픈 */
				inputFile.open(file.c_str(), ifstream::in);
				if (inputFile.bad())
					continue;

				info.resize(nLine / 3);
				imgIdxColl.resize(nLine - (nLine / 3));
				imgDataColl.resize(nLine / 3);
				for (int i = 0; i < nLine - (nLine / 3); ++i)
					imgIdxColl[i].resize(1300);

				try
				{
					/* 버젼넘버, 타일의 가로 및 세로크기, 이미지 데이터, 이미지 데이터 인덱스,
					 * Doodad 조합정보를 읽어옴.
					 */
					LoadVersionNum(inputFile, thm);
					LoadTileWidth(inputFile, thm);
					LoadTileHeight(inputFile, thm);
					LoadNumOfCombo(inputFile);
					LoadComboNames(inputFile);
					LoadImageData(inputFile, thm);
					LoadImageIndex(inputFile, thm);
					LoadImageIndex(inputFile, thm);
					LoadDoodadInfo(inputFile, thm);
				}
				catch (logic_error msg)
				{
					/* 실패하였을 경우 로그정보를 기록한다. */
					logFile.write(msg.what(), strlen(msg.what()));
					fileList.close();
				}

				inputFile.close();
			}
			fileList.close();
		}
	}

	/* ifstream에서 테마 값을 읽어들임 */
	std::ifstream& ImgResource::ReadTheme(std::ifstream &in, Theme &thm)
	{
		string line;
		
		getline(in, line);
		if (line == "GRASS")
			thm = GRASS;
		else if (line == "STON")
			thm = STON;
		else if (line == "CASTLE")
			thm = CASTLE;

		themeList.push_back(line);
		return in;
	}

	/* 만약 텍스트 파일에서 한 라인에 int형 데이터가 하나만 있을 경우에
	 * 사용되는 메소드로 코드의 라인을 줄이기 위해 사용되었다.
	 */
	ifstream& ImgResource::GetOneValueInLine(ifstream &input, int &container, string errMsg)
	{
		string line;
		getline(input, line);
		if (input.good() == false)
			throw logic_error(errMsg);
		container = atoi(line.c_str());

		return input;
	}

	/* 사용자가 넘겨준 mainKey, imgHash 매개변수를 이용하여
	 * 해당 이미지 인덱스에 대한 실제 이미지 데이터를 사용자가
	 * 넘겨준 container 담아서 리턴한다. 이미지 인덱스가 유효하지
	 * 않다면 false를 리턴함.
	 */
	bool ImgResource::GetImageData(const int mainKey, const int imgHash, 
								   ImageData &container)
	{
		int idx = ImageIndex(mainKey, imgHash);
		if (idx == -1)
		{
			logFile.write("GetImageData(): 유효하지 않은 이미지 인덱스\n",
						  strlen("GetImageData(): 유효하지 않은 이미지 인덱스\n"));
			return false;
		}

		memcpy(&container, &imgDataColl[theme][idx], sizeof(ImageData));
		return true;
	}

	bool ImgResource::GetImageData(const int imgIdx, ImageData &container)
	{
		if (imgIdx < 0 || imgIdx >= static_cast<int>(imgDataColl[theme].size()))
		{
			logFile.write("GetImageData() (using index): 유효하지 않은 이미지 인덱스\n",
						  strlen("GetImageData() (using index): 유효하지 않은 이미지 인덱스\n"));
			return false;
		}

		memcpy(&container, &imgDataColl[theme][imgIdx], sizeof(ImageData));
		return true;
	}

	/* 사용자가 넘겨준 container에 버튼으로 쓰이는 이미지들의
	 * 정보를 채워준다.
	 */
	void ImgResource::GetBtnImgData(vector<ImageData> &container, vector<int> &idx)
	{
		vector<ImageData>::iterator iter = imgDataColl[theme].begin();
		DistinctionBtn op = DistinctionBtn();
		
		for ( ; iter != imgDataColl[theme].end(); ++iter)
		{
			iter = find_if(iter, imgDataColl[theme].end(), op);

			if (iter == imgDataColl[theme].end())
				break;
				
			container.push_back((*iter));
			container.back().width		= container.back().width - container.back().x;
			container.back().height		= container.back().height - container.back().y;
			idx.push_back(distance(imgDataColl[theme].begin(), iter));
		}
	}

	/* 사용자가 요구한 Doodad에 대한 정보를 container에 담아서 리턴 */
	bool ImgResource::GetDoodadData(const int imgIdx, ImageData &container)
	{
		//if (imgIdx < info[static_cast<int>(theme)].doodadStartNum ||
		//	imgIdx >= info[static_cast<int>(theme)].doodadStartNum + DoodadDataColl.size())
		//{
		//	logFile.write("GetDoodadData() (using index): 유효하지 않은 이미지 인덱스\n",
		//				  strlen("GetDoodadData() (using index): 유효하지 않은 이미지 인덱스\n"));
		//	return false;
		//}
		memcpy(&container, &imgDataColl[theme][imgIdx], sizeof(ImageData));
		
		/* x, y 중심점을 계산 */
		container.centerPointY = container.y - container.centerPointY;
		container.centerPointX = container.x - container.centerPointX;

		return true;
	}

	/* 사용자가 넘겨준 타일 및 이미지 인덱스를 이용하여 타일 위에
	 * 이미지가 올려질 수 있는지 여부를 리턴한다.
	 */
	bool ImgResource::AvailableBrush(const int tileIdx, const int imgIdx)
	{
		if (tileIdx >= static_cast<int>(imgDataColl[theme].size()) || 
			imgIdx >= static_cast<int>(imgDataColl[theme].size()))
		{
			logFile.write("AvailableBrush(): 유효하지 않은 타일 또는 이미지 인덱스\n",
						  strlen("AvailableBrush(): 유효하지 않은 타일 또는 이미지 인덱스\n"));
			return false;
		}
		if (imgDataColl[theme][imgIdx].availableTile == -1 ||
			imgDataColl[theme][imgIdx].availableTile == tileIdx)
			return true;
		return false;
	}

	/* 사용자가 넘겨준 mainKey, imgHash 매개변수를 이용하여
	 * 해쉬맵에 저장되어 있는 이미지의 인덱스를 얻어온다.
	 * 만약 이미지 인덱스가 존재하지 않는다면 -1을 리턴함.
	 */
	int ImgResource::ImageIndex(const int mainKey, const int imgHash)
	{
		return imgIdxColl[mainKey + (theme * 2)][imgHash];
		//return imgIdxColl[mainKey][imgHash];
	}

	/* Doodad 조합이 가능하면 true를 아니면, false를 리턴. */
	bool ImgResource::DoodadAvailable(const int idx, const int imgIdx)
	{
		if ((idx - info[theme].doodadStartNum) < 0 ||
			(idx - info[theme].doodadStartNum) >= static_cast<int>(DoodadDataColl.size()))
		{
			logFile.write("DoodadAvailable(): 유효하지 않은 Doodad 인덱스\n",
						  strlen("DoodadAvailable(): 유효하지 않은 Doodad 인덱스\n"));
			return false;
		}
		vector<int>::iterator iter;
		iter = find(DoodadDataColl[idx - info[theme].doodadStartNum].begin(), 
					DoodadDataColl[idx - info[theme].doodadStartNum].end(), imgIdx);

		if (iter == DoodadDataColl[idx - info[theme].doodadStartNum].end())
			return false;
		return true;
	}

	bool ImgResource::IsDoodad(const int imgIdx)
	{
		if (imgIdx >= info[theme].doodadStartNum && 
			imgIdx <= info[theme].doodadStartNum + static_cast<int>(DoodadDataColl.size()))
			return true;
		return false;
	}

	bool ImgResource::CheckImageType(int imgIdx, ImageType type)
	{
		if (imgIdx < 0 || imgIdx > static_cast<int>(imgIdxColl[theme * 2].size()))
		{
			logFile.write("CheckImageType(): 유효하지 않은 이미지 인덱스\n",
						  strlen("CheckImageType(): 유효하지 않은 이미지 인덱스\n"));
			return false;
		}
		if (imgDataColl[theme][imgIdx].type == type)
			return true;
		return false;
	}

	/* 콤보박스의 Name을 읽어들임 */
	ifstream& ImgResource::LoadComboNames(ifstream &input)
	{
		ComboNames.reserve(nComboBox);
		string name;
		for (int i = 0; i < nComboBox; ++i)
		{
			getline(input, name);
			if (input.bad() == true)
			{
				logFile.write("LoadComboNames(): 콤보박스 이름 읽어들이기 실패\n",
							  strlen("LoadComboNames(): 콤보박스 이름 읽어들이기 실패\n"));
				return input;
			}
			ComboNames.push_back(name);
		}
		return input;
	}

	/* 콤보박스의 개수를 읽어들임 */
	ifstream& ImgResource::LoadNumOfCombo(ifstream &input)
	{
		GetOneValueInLine(input, nComboBox, "LoadNumOfCombo(): 콤보박스 개수 읽어들이기 실패\n");
		return input;
	}

	/* 버젼넘버를 읽어들인다. */
	ifstream& ImgResource::LoadVersionNum(ifstream &input, int thm)
	{
		GetOneValueInLine(input, info[thm].version, "LoadVersionNum(): 버젼넘버 읽어들이기 실패\n");
		return input;
	}

	/* 타일의 가로길이를 읽어들인다. */
	ifstream& ImgResource::LoadTileWidth(ifstream &input, int thm)
	{
		GetOneValueInLine(input, info[thm].width, "LoadTileWidth(): 타일의 가로길이 읽어들이기 실패\n");
		return input;
	}

	/* 타일의 세로길이를 읽어들인다. */
	ifstream& ImgResource::LoadTileHeight(ifstream &input, int thm)
	{
		GetOneValueInLine(input, info[thm].height, "LoadTileHeight(): 타일의 세로길이 읽어들이기 실패\n");
		return input;
	}

	/* 이미지 데이터를 모두 읽어들인다. */
	ifstream& ImgResource::LoadImageData(ifstream &input, int thm)
	{
		int nImgData = 0;

		GetOneValueInLine(input, nImgData, "LoadImageData(): 이미지 데이터 개수 읽어들이기 실패\n");
		ImageData data;
		string line;
		vector<string> splitedStr;
		for (int i = 0; i < nImgData; ++i)
		{
			getline(input, line);
			if (input.good() == false)
				throw logic_error("LoadImageData(): 이미지 데이터의 형식이 잘못됨\n");

			/* 탭을 구분자로 문자열을 쪼갰을 때, splitedStr벡터가 비어있다면,
			 * 이미지 리소스 정보를 담고있는 텍스트 파일에 엔터문자가 들어
			 * 있을경우이다.
			 */
			SplitString(line, splitedStr);
			if (splitedStr.empty() == true)
				throw logic_error("LoadImageData(): 이미지 데이터 읽어들이기 실패\n");
			
			data.x				= atoi(splitedStr[0].c_str());
			data.y				= atoi(splitedStr[1].c_str());
			data.width			= atoi(splitedStr[0].c_str()) + atoi(splitedStr[2].c_str());
			data.height			= atoi(splitedStr[1].c_str()) + atoi(splitedStr[3].c_str());
			data.moveable		= (atoi(splitedStr[4].c_str()) == 1)? true : false;
			data.centerPointX	= atoi(splitedStr[0].c_str()) + atoi(splitedStr[5].c_str());
			data.centerPointY	= atoi(splitedStr[1].c_str()) + atoi(splitedStr[6].c_str());
			data.availableTile	= atoi(splitedStr[7].c_str());
			data.type			= atoi(splitedStr[8].c_str());
			data.isBtn			= atoi(splitedStr[9].c_str());
			data.tileType		= atoi(splitedStr[10].c_str());
			data.shadow			= atoi(splitedStr[11].c_str());

			/* 만약 대표타일이라면... */
			if (data.type == 0)
			{
				for_each(imgIdxColl[data.tileType].begin(), imgIdxColl[data.tileType].end(),
						 ValueGenerator(i));
			}

			imgDataColl[thm].push_back(data);

			line.clear();
			splitedStr.clear();
		}
		return input;
	}

	/* 이미지 인덱스 정보를 모두 읽어들인다. */
	ifstream& ImgResource::LoadImageIndex(ifstream &input, int thm)
	{
		int mainKey;
		int nImgKey;

		GetOneValueInLine(input, mainKey, "LoadImageIndex(): 메인키 읽어들이기 실패\n");
		GetOneValueInLine(input, nImgKey, "LoadImageIndex(): 서브키의 개수 읽어들이기 실패\n");
		
		string line;
		vector<string> splitedStr;
		for (int i = 0; i < nImgKey; ++i)
		{
			getline(input, line);
			if (input.good() == false)
				throw logic_error("LoadImageIndex(): 이미지 인덱스 읽어들이기 실패\n");

			/* 탭을 구분자로 문자열을 쪼갰을 때, splitedStr벡터가 비어있다면,
			 * 이미지 리소스 정보를 담고있는 텍스트 파일에 엔터문자가 들어
			 * 있을경우이다.
			 */
			SplitString(line, splitedStr);
			if (splitedStr.empty() == true)
				throw logic_error("LoadImageIndex(): 이미지 인덱스의 형식이 잘못됨\n");
			//tempColl.insert(make_pair(atoi(splitedStr[0].c_str()), atoi(splitedStr[1].c_str())));
			imgIdxColl[mainKey + (thm * 2)][atoi(splitedStr[0].c_str())] = atoi(splitedStr[1].c_str());

			line.clear();
			splitedStr.clear();
		}
		//imgIdxColl.insert(make_pair(mainKey, tempColl));
		return input;
	}

	/* Doodad 정보를 모두 읽어들인다. */
	ifstream& ImgResource::LoadDoodadInfo(std::ifstream &input, int thm)
	{
		GetOneValueInLine(input, info[thm].nDoodad, "LoadDoodadInfo(): Doodad의 개수 읽어들이기 실패\n");
		GetOneValueInLine(input, info[thm].doodadStartNum, "LoadDoodadInfo(): Doodad의 개수 읽어들이기 실패\n");
		DoodadDataColl.reserve(info[thm].nDoodad);

		string line;
		vector<string> splitedStr;
		for (int i = 0; i < info[thm].nDoodad; ++i)
		{
			getline(input, line);
			if (input.good() == false)
				throw logic_error("LoadDoodadInfo(): Doodad 정보 읽어들이기 실패\n");

			/* 탭을 구분자로 문자열을 쪼갰을 때, splitedStr벡터가 비어있다면,
			 * 이미지 리소스 정보를 담고있는 텍스트 파일에 엔터문자가 들어
			 * 있을경우이다.
			 */
			SplitString(line, splitedStr);
			if (splitedStr.empty() == true)
				throw logic_error("LoadDoodadInfo(): Doodad의 형식이 잘못됨\n");

			int key = atoi(splitedStr[0].c_str());
			vector<int> tempColl;
			tempColl.reserve(splitedStr.size() - 1);
			for (unsigned int j = 1; j < splitedStr.size(); ++j)
				tempColl.push_back(atoi(splitedStr[j].c_str()));

			DoodadDataColl.push_back(tempColl);

			line.clear();
			splitedStr.clear();
		}
		return input;
	}

	/* 탭을 구분자로 하여 문자열을 쪼갠다. */
	void ImgResource::SplitString(string &str, vector<string> &coll)
	{
		string::iterator startPos, endPos;

		startPos = str.begin();
		endPos = startPos;
		while (endPos != str.end())
		{
			if (*endPos == '\t' && endPos != str.begin())
			{
				coll.push_back(string(startPos, endPos));
				startPos = endPos + 1;
			}
			++endPos;
			if (endPos == str.end() && startPos != endPos)
				coll.push_back(string(startPos, endPos));
		}
	}

	void ImgResource::GetPillar(const int tileType, ImageData &container, bool top)
	{
		if (top == true)
			GetImageData(tileType, 1296, container);
		else
			GetImageData(tileType, 1297, container);
	}

	int ImgResource::GetShadowIndex(const int tileType, const int doodadIdx)
	{
		int shadow = imgDataColl[theme][doodadIdx].shadow;
		if (shadow == 1)
			return ImageIndex(tileType, 1298);
		else if (shadow == 2)
			return ImageIndex(tileType, 1299);
		else
			return 0;
	}

	ImgResource* ImgResource::Instance()
	{
		static ImgResource singleton;

		return &singleton;
	}
} /* namespace ProjectH */