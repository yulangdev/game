// About.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ProjectH_MapEdit.h"
#include "About.h"


// About 대화 상자입니다.

IMPLEMENT_DYNAMIC(About, CDialog)

About::About(CWnd* pParent /*=NULL*/)
	: CDialog(About::IDD, pParent)
{

}

About::~About()
{
}

void About::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, thanksTo);
	DDX_Control(pDX, IDC_EDIT2, comment);
}


BEGIN_MESSAGE_MAP(About, CDialog)
END_MESSAGE_MAP()


// About 메시지 처리기입니다.

BOOL About::OnInitDialog()
{
	CDialog::OnInitDialog();

	comment.SetWindowText( L"  이 프로그램은 개인적인 공부를 목적으로 HCI연구실에 있는 04오영석과 05유종원이 만들었습니다. \r\n\r\n  본 프로그램에 쓰인 이미지 리소스들은 TGL사의 파랜드 택틱스 1, 2 의 리소스들입니다. 따라서, 이미지에 대한 저작권이 저희에게 없기 때문에 배포는 하지 말아주셨으면 좋겠습니다. 2d게임을 공부하시거나 맵툴을 만들고 계시는 분들에게 조금이나마 도움이 됐으면 좋겠습니다.\r\n\r\n  연락이 필요하시면 oys8601@nate.com 이나 dirtyschool1@naver.com 으로 메일을 보내주세요." );

	thanksTo.SetWindowText( L"  우리 같이 연구실에서 공부했던 정식이형 태섭이형 상현이형 고마워요 ^_^ 태섭이형 개발업계는 살만한 곳일지도 몰라요. 다시 생각해보고 내년에 서울에서 만나요!! 상현이형 여자친구 생겨서 좋겠어요ㅋ 이제 던파할일은 없겠네요?! 그럼 이제 이건 내꺼?!?!ㅋㅋ 잘쓸께요~/ 정식이형 얼른 취업하셔야 될텐데.. 어디든 할 수 있겠지만, 이제 3월이 지나가는데 은근히 걱정되네요 ㅠㅠ 기다린 만큼 좋은데 취업하길 바래요!! 영중이형 간간히 원격으로도 도와주시구 항상 감사해요ㅋ 형의 내공에는 정말 끝이 없는것 같아요~ (__) 이제 내공은 그만쌓고 취업하시면 정말정말 좋을 것 같아요~/ㅋㅋ 그리구 허광일선배님 감사합니다. 광일이형이 스승의날에 저에게 dx코드설명 안해주셨으면 감히 이런건 꿈또 못꿨을꺼예요ㅎㅎ 정말 졸업하기 전에 한번 만들어보고 싶었는데, 대단한거 만든건 아니지만.. 광일이형 덕분에 이렇게 만들 수 있게 되었어요! 정말 감사합니다. (__) 그리고 저희에게 공부할 수 있는 자리를 마련해주신 조상교수님께도 감사드려요! 마지막으로 저와 같이 프로젝트를 진행하고 있는 영석이형 제가 이런 프로젝트가 처음이라 이랬다 저랬다 해서 짜증도 많이 났을텐데 아직 반밖엔 안왔지만 지금까지 같이 와줘서 정말 고마워요! 앞으로 우리 남은 부분도 열심히 해서 꼭 이 프로젝트에 마침표를 찍자구요~!ㅋ From 유종원.\r\n\r\n"
L"  이번에 이 맵툴을 만들게 되면서 사실 MFC나 DX를 처음 써보는 거라서 많은 걱정이 있었는데, 같은 파트너인 유종원군이 이 부분의 인터페이스를 잘 만들어주어서 크게 도움이 되었던것 같다.  부족한 나와 같이 이번 프로젝트를 진행한 유종원군에게 일단 고맙다고 말하고 싶다.  그리고 로직을 짜는데 있어서 중요고비마다 도움을 주신 교수님에게도 정말 큰 감사를 드리고 싶고, 같은 연구실에서 공부하고 있는 정식이형, 현업에서 프로그래머로 일하고 있는 태섭이형, 상현이형등 항상 주말마다 잡담을 하면서 C++의 문법에서부터 이런저런 기술들에 대해서 이야기를 하면서 서로의 지식을 공유하면서 알게 된 것들이 참 많은 도움이 되었다. From 오영석." );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
