#pragma once
#include <list>
#include <vector>
#include <string>

#include "ImgResource.h"
#include "Navigation.h"
#include "BlackWindow.h"

#define GRASS_BTN_IMG		"Image/Grass.png"
#define GRASS_BTN_INFO		"Data/grassinfo.txt"

class BrushPalette : public CDialog
{
	DECLARE_DYNAMIC(BrushPalette)

public:
	BrushPalette(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~BrushPalette();

public:
	Navigation* navigation;
	BlackWindow* blackBg;

public:
	CComboBox m_ComboBox;				// 타일, 악세서리 분류 콤보박스

	CButton m_BtnCheckAutoTiling;		// 오토타일링 체크 버튼

	CImage m_TileImage;					// 불러온 타일 종합 이미지
	CImage startPoint;					// StartPoint 이미지
	CImage monster;						// StartPoint 이미지

	CButton tileSelect;					// 기본 마우스 버튼
	CButton tileHeightUp;				// 타일 높이 올리는 버튼
	CButton tileHeightDown;				// 타일 높이 내리는 버튼
	CButton elevationText;

	void ReleaseButtonList(bool releaseStartPoint = false);			// 버튼 해제

	void OnComboBoxChange() { AdjustBtnPosition(); AdjustStartPointPos(); }			// 콤보박스 선택
	void OnRadioButtonClicked();		// 타일 이미지 버튼 클릭

	void LoadImgAndCreateBtns();						/* 버튼생성 */
	int GetSelectedIndex() { return selectedBtnIdx; }	/* 사용자가 선택한 버튼의 이미지 인덱스 리턴 */
	int GetSelectedKey() { return selectedBtnKey; }		/* 사용자가 선택한 버튼의 이미지 키 리턴 */

	bool IsAutoTiling() { return !( m_BtnCheckAutoTiling.GetCheck() ); } /* 오토타일링 버튼 체크상태 리턴 */

	bool PlayerBtnClicked() const { return playerBtnClicked; }
	int GetSelectedPlayer() const { return selectedPlayer; }

	bool MonsterBtnClicked() const { return monsterBtnClicked; }
	int GetSelectedMonster() const { return selectedMonster; }

	void Move( int x, int y );

private: /* Define Struct, enum ... */
	
	/* 이미지 정보를 저장하는 구조체 */
	struct BtnImgInfo 
	{
		CButton *button;		/* 실제 버튼 */
		unsigned int ComboIdx;	/* 콤보박스 인덱스 */
		unsigned int ImgIdx;	/* 이미지 인덱스 */
		unsigned int x, y;		/* 시작점 x, y */
		unsigned int w, h;		/* 가로, 세로 */
	};

private: /* Variables */
	std::list<BtnImgInfo*> Buttons;
	std::list<CImage*> buttonsImg;
	int selectedBtnIdx;						/* 사용자가 선택한 버튼의 이미지 인덱스 */
	int selectedBtnKey;						/* 사용자가 선택한 버튼의 이미지 키 */
	
	bool playerBtnClicked;					/* Player StartPoint 버튼의 선택여부 */
	int selectedPlayer;						/* 선택된 플레이어 */

	bool monsterBtnClicked;					/* Player StartPoint 버튼의 선택여부 */
	int selectedMonster;						/* 선택된 플레이어 */

	std::vector<CButton*> playerBtns;		/* Player1 ~ Player8 StartPoint버튼 */
	std::list<CImage*> playerBtnsImg;

	std::vector<CButton*> monsterBtns;		/* Player1 ~ Player8 StartPoint버튼 */
	std::list<CImage*> monsterBtnsImg;

private: /* Methods */
	void ReadOverallImage();								/* 버튼에 사용할 이미지를 위한 전체 이미지를 불러옴. */
	bool ReadImgInfo(std::list<BtnImgInfo*>&);				/* 텍스트 파일로부터 이미지 정보를 모두 읽어들임. */
	bool CreateButtons(std::list<BtnImgInfo*>&);			/* 버튼 이미지 정보를 바탕으로 실제 버튼을 생성. */

	void SplitString(std::string, std::vector<std::string>&);	/* 탭을 구분자로 하여 문자들을 쪼갬. */
	bool CheckLine(std::vector<std::string>&);					/* 이미지 정보를 제대로 읽어들였는지 검사. */
	
	void AdjustBtnPosition();
	void AdjustStartPointPos();

	void LoadStartPointImage();
	void CreateStartPointBtns();

	void LoadMonsterImage();
	void CreateMonsterBtns();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnMove(int x, int y);
};
