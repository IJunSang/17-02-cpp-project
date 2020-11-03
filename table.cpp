#include <iostream>
#include <stdlib.h>
#include <cstdio>
#include <fstream>
#include <conio.h>
#include <ctime>
#include "libxl.h"
#include "SerialClass.h"

using namespace std;
using namespace libxl;

class Worker
{
private:
	char name[20];				//이름
	char position;			//직급(1. 사장, 2. 점장, 3. 직원, 4. 알바)
	int working_time;		//주당 일하는 시간
	bool work_time[24] = { false, };		//일하는 시간 
	bool work_day[31];		//일하는 요일
	char status[31];		//상태를 나타내는 변수(1. 출근, 2. 결근, 3. 조퇴, 4. 지각)
	int cardInfo;
	bool in_check = false, out_check = false;
public:
	// private 변수들을 저장하고 불러오게 해주는 get, set함수 구현
	void SetName(char n[20])
	{
		strcpy(this->name, n);
	}
	void SetPosition(char p)
	{
		this->position = p;
	}
	void SetWorkingTime(int time)
	{
		this->working_time = time;
	}
	void SetWorkTime(int start, int end)
	{
		for (int i = start; i <= end; i++)
		{	// 출근 시간을 start, 퇴근 시간을 end에 담아 bool 배열에 일하는 해당 시간만 true로 저장
			work_time[i] = true;
		}
	}
	void SetCardInfo(char *c)
	{	// 시리얼통신을 통해 얻은 값을 본 객체의 cardInfo변수에 저장
		char temp[256];
		strcpy(temp, c);
		cardInfo = atoi(temp);
	}
	void SetInCheck(bool check)
	{
		this->in_check = check;
	}
	void SetOutCheck(bool check)
	{
		this->out_check = check;
	}
	char* GetName()
	{
		return this->name;
	}
	char GetPosition()
	{
		return this->position;
	}
	int GetWorkingTime()
	{
		return this->working_time;
	}
	bool GetWorkTime(int i)
	{
		return work_time[i];
	}
	bool GetWorkDay(int i)
	{
		return work_day[i];
	}
	int GetCardInfo()
	{
		return this->cardInfo;
	}
	bool GetInCheck()
	{
		return this->in_check;
	}
	bool GetOutCheck()
	{
		return this->out_check;
	}
	void SetSchedule()
	{
		time_t t = time(NULL);
		struct tm *now = localtime(&t);
		int select, input;
		char *days[] = { "SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT" };

		cout << name << "의 스케줄을 생성합니다." << endl;
		cout << "이번주 스케줄을 작성/수정 하려면 0, 다음주 스케줄을 작성하려면 1을 누르세요 : ";
		cin >> select;
		int showDay = 6 - (now->tm_wday);
		int temp = now->tm_mday;
		cout << "해당하는 요일에 근무하고싶으면 1, 아니면 0을 입력하세요" << endl;

		switch(select)
		{
		case 0:
			for (int i = 0; i < 7; i++)
			{
				printf(" %3s |", days[i]);
			}
			printf("\n");
			temp -= (now->tm_wday);
			for (int i = 0; i < 7; i++)
			{
				if (temp> 31)
					break;
				if (temp <= 0)
					printf("%5c|", ' ');
				else
					printf("%5d|", temp);
				temp++;
			}
			printf("\n");
			for (int i = 0; i < (now->tm_wday); i++)
			{
				printf("     |");
			}
			for (int i = (now->tm_mday); i <= ((now->tm_mday) + showDay); i++)
			{
				if (i > 31)
					break;
				printf("%4c", ' ');
				input = getche();
				if (input == '0')
					work_day[i] = false;
				else
					work_day[i] = true;
				printf("|");
			}
			printf("\n");
			break;
		case 1:
			temp = 7 - (now->tm_wday);
			for (int i = 0; i < 7; i++)
			{
				printf(" %3s |", days[i]);
			}
			printf("\n");
			for (int i = (now->tm_mday) + temp; i < ((now->tm_mday) + temp + 7); i++)
			{
				if (i > 31)
					break;
				else if (i <= 0)
					printf("%5c|", ' ');
				else
					printf("%5d|", i);
			}
			printf("\n");
			for (int i = (now->tm_mday) + temp; i < (now->tm_mday) + temp + 7; i++)
			{
				if (i > 31)
					break;
				printf("%4c", ' ');
				input = getche();
				if (input == '0')
					work_day[i] = false;
				else
					work_day[i] = true;
				printf("|");
			}
			printf("\n");
			break;
		default:
			cout << "잘못 입력했습니다." << endl;
		}
	}
	void SetSchedule(int day, bool work)
	{
		work_day[day] = work;
	}
	bool CmpName(char name[])
	{
		int result = strcmp(this->name, name);
		if (result == 0)
			return true;
		else
			return false;
	}
	void PrintSchedule()
	{
		for (int i = 0; i < 31; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				printf("%3d|", i);
			}
			printf("\n");
			for (int j = 0; j < 7; j++, i++)
			{
				if (work_day[i])
					printf("%3c|", 'O');
				else
					printf("%3c|", 'X');
			}
			printf("\n");
		}
	}
};

class ListNode
{
public:
	ListNode *next;
	Worker *w;
	ListNode()
	{
		this->next = NULL;
	}
	ListNode(Worker *w)
	{
		this->w = w;
		this->next = NULL;
	}
};

class LinkedList
{
	ListNode *head;
	int count = 0;
public:
	LinkedList() { this->head = NULL; }
	void insertNode(Worker *w)
	{
		ListNode *n = new ListNode(w);
		if (head == NULL)
		{
			head = n;
		}
		else
		{
			n->next = head;
			head = n;
		}
		count++;
	}
	void DeleteNode(ListNode *n)
	{
		ListNode *temp = head;
		if (head == NULL)
		{
			cerr << "리스트가 비어있습니다!" << endl;
			return;
		}
		else if (head == n)
		{
			if (head->next != NULL)
				head = head->next;
			else
				head = NULL;
			delete temp;
			count--;
			return;
		}
		while (temp->next != NULL)
		{
			if (temp->next == n)
			{
				ListNode *tmp = temp->next;

				temp->next = temp->next->next;
				delete tmp;
			}
			else
			{
				temp = temp->next;
			}
		}
	}
	ListNode *SearchNode(char name[20])
	{
		ListNode *temp = head;
		while (temp != NULL)
		{
			if (temp->w->CmpName(name))
				return temp;
			else
				temp = temp->next;
		}

		return NULL;
	}
	ListNode *searchNode(int count)
	{
		ListNode *temp = head;
		if (temp == NULL)
		{
			cerr << "리스트가 비었습니다!" << endl;
			return NULL;
		}
		for (int i = 0; i < count; i++)
		{
			temp = temp->next;
		}
		return temp;
	}
	void PrintList()
	{
		ListNode *temp = head;
		if (temp == NULL)
		{
			cout << "리스트가 비어있습니다!" << endl;
			return;
		}
		while (temp != NULL)
		{
			printf("%s", temp->w->GetName());
			switch (temp->w->GetPosition())	//(1. 사장, 2. 점장, 3. 직원, 4. 알바)
			{
			case 1:
				cout << "사장";
				break;
			case 2:
				cout << "점장";
				break;
			case 3:
				cout << "직원";
				break;
			case 4:
				cout << "알바";
				break;
			}
			cout << "주당 근무시간 : " << (temp->w->GetWorkingTime()) << endl;
			temp = temp->next;
		}
	}
	int GetCount()
	{
		return this->count;
	}
	void StoreInfo()
	{
		ListNode *temp = head;
		ofstream fout;
		fout.open("Info.txt", ios::out);
		while (temp != NULL)
		{
			fout.write((char*)(temp->w), sizeof(Worker));
			temp = temp->next;
		}
		fout.close();
	}
	void LoadInfo()
	{
		ifstream fin;
		int count = 1;
		Worker w;
		fin.open("Info.txt", ios::in);
		if (!fin)
		{
			cerr << "파일 열기 오류!" << endl;
			return;
		}
		fin.read((char *)&w, sizeof(Worker));
		insertNode(&w);
		fin.close();
	}
	bool CheckError(int work_min, int open_hour, int close_hour)
	{
		ListNode *temp = head;
		int cnt = 0;
		time_t t = time(0);
		struct tm *now = localtime(&t);
		bool **work = new bool*[count];					//모든 직원의 시간표를 저장, 해당 시간에 일하면 true, 아니면 false
		int restWorker[10] = { 0, };
		bool test = true;
		if (temp == NULL)
		{
			cout << "직원 리스트가 비어있습니다!!" << endl;
			return false;
		}
		cout << "출/퇴근 시간 문제 확인" << endl;
		for (int i = 0; i < count; i++)
		{
			work[i] = new bool[24];
			for (int j = 0; j < 24; j++)
			{
				work[i][j] = temp->w->GetWorkTime(j);
			}
			temp = temp->next;
		}
		for (int i = open_hour; i < close_hour; i++)
		{
			cnt = 0;
			for (int j = 0; j < count; j++)
			{
				if (work[j][i] == true)
					cnt++;
			}
			if (cnt < work_min)
			{
				cerr << i << "시에 일하는 직원 수가 매장에 있어야 할 최소 직원 수보다 작습니다" << endl;
				test = false;
			}
		}
		if (test == true)
			cout << "시간 문제 없음" << endl;

		temp = head;
		for (int i = 0; i < count; i++)
			delete work[i];
		cout << (now->tm_mday + 1) << "일부터 " << (now->tm_mday + 8) << "일 까지 문제 확인" << endl;
		for (int i = 0; i < count; i++)
		{
			work[i] = new bool[31];
			for (int j = (now->tm_mday + 1); j <= (now->tm_mday + 8); j++)
			{
				work[i][j] = temp->w->GetWorkDay(j);
			}
			temp = temp->next;
		}
		for (int i = (now->tm_mday + 1); i <= (now->tm_mday + 8); i++)
		{
			cnt = 0;
			int index = 0;
			for (int j = 0; j < count; j++)
			{
				if (work[j][i] == true)
					cnt++;
				else
					restWorker[index++] = j;
			}
			if (cnt < work_min)
			{
				cerr << i << "일에 일하는 직원 수가 매장에 있어야 할 최소 직원 수보다 작습니다." << endl;
				for (int j = 0; j < index; j++)
				{
					cout << this->searchNode(restWorker[j])->w->GetName() << "(이)가 "<<i<<"일에 쉽니다." << endl;
				}
				test = false;
			}
		}
		if (test == true)
			cout << "날짜 문제 없음" << endl;
		return test;
	}
	bool MakeTimeTable()
	{
		time_t t = time(NULL);
		struct tm *now = localtime(&t);
		int year = now->tm_year + 1900;
		int mon = now->tm_mon + 1;
		char day = 'D';
		wchar_t *send = new wchar_t();
		int row = 5;
		int base_col = 1;

		Book* book = xlCreateBook();
		Sheet* sheet = book->addSheet(L"Sheet1");
		Font* font = book->addFont();

		font->setSize(22);

		Format* format = book->addFormat();
		format->setFont(font);
		format->setAlignH(ALIGNH_CENTER);
		format->setAlignV(ALIGNV_CENTER);

		swprintf(send, L"%d년도 %d월 출근부", year, mon);
		sheet->writeStr(1, 3, send, format);

		sheet->setMerge(1, 1, 3, 33);

		font = book->addFont();
		font->setSize(10);
		format = book->addFormat();
		format->setFont(font);
		format->setAlignH(ALIGNH_CENTER);
		format->setAlignV(ALIGNV_CENTER);
		sheet->writeStr(3, 1, L"직종", format);
		sheet->writeStr(3, 2, L"성명", format);

		sheet->setMerge(3, 4, 1, 1);
		sheet->setMerge(3, 4, 2, 2);


		font = book->addFont();
		font->setSize(8);
		format = book->addFormat();
		format->setFont(font);
		format->setAlignH(ALIGNH_CENTER);
		format->setAlignV(ALIGNV_CENTER);
		format->setNumFormat(book->addCustomNumFormat(L"d"));

		for (int i = 1; i <= 31; i++)
		{
			swprintf(send, L"DATE(%d, %d, %d)", year, mon, i);

			sheet->writeFormula(3, (2 + i), send, format);
		}
		format = book->addFormat();
		format->setNumFormat(book->addCustomNumFormat(L"aaa"));
		for (int i = 1; i <= 31; i++)
		{
			swprintf(send, L"DATE(%d,%d,%d)", year, mon, i);
			sheet->writeFormula(4, (2 + i), send, format);
		}
		sheet->setCol(0, 0, 1);
		sheet->setCol(3, 33, 3);

		format = book->addFormat();
		format->setAlignH(ALIGNH_CENTER);
		format->setAlignV(ALIGNV_CENTER);

		ListNode *temp = head;
		while (temp != NULL)
		{
			base_col = 1;
			char c = temp->w->GetPosition();
			switch(c)
			{
			case '1':
				swprintf(send, L"사장");
				break;
			case '2':
				swprintf(send, L"점장");
				break;
			case '3':
				swprintf(send, L"직원");
				break;
			case '4':
				swprintf(send, L"알바");
				break;
			}
			sheet->writeStr(row, base_col++, send, format);

			char *tmp = new char[strlen(temp->w->GetName())];
			sprintf(tmp, "%s", temp->w->GetName());
			wchar_t *send1 = new wchar_t;
			mbstowcs(send1, tmp, strlen(tmp) + 1);
			sheet->writeStr(row, base_col, send1, format);

			for (int i = 0; i < 31; i++)
			{
				if (temp->w->GetWorkDay(i))
					swprintf(send, L"O");
				else
					swprintf(send, L"X");
				sheet->writeStr(row, (3 + i), send, format);
			}

			temp = temp->next;
			row++;
		}

		sheet->writeStr(3, 34, L"출근시간", format);
		sheet->writeStr(3, 35, L"퇴근시간", format);

		book->save(L"test.xls");

		book->release();

		return true;
	}
	void StartCheck()
	{
		Serial *rfid = new Serial("\\\.\\COM9");
		ListNode *temp = head;
		Book *book = xlCreateBook();
		book->load(L"test.xls");
		Sheet *sheet = book->getSheet(0);
		wchar_t send[20];

		if (temp == NULL)
		{
			cout << "리스트가 없습니다!" << endl;
			return;
		}
		char incomingData[256] = "";
		int readResult, cardInfo, row;
		if (rfid->IsConnected())
			cout << "출근 체크를 시작합니다." << endl;
		else
			cerr << "센서가 감지되지 않았습니다. 다시 시도하세요" << endl;
		while (rfid->IsConnected())
		{
			while (!strcmp(incomingData, ""))
			{
				readResult = rfid->ReadData(incomingData, 256);
			}
			incomingData[readResult] = 0;
			cardInfo = atoi(incomingData);
			cout << "읽은 카드값 : " << cardInfo << endl;
			row = 5;
			time_t t = time(NULL);
			struct tm *now = localtime(&t);

			while(temp != NULL)
			{
				int card = temp->w->GetCardInfo();
				if (card == cardInfo)
				{
					swprintf(send, L"%d:%d", now->tm_hour, now->tm_min);
					if (!temp->w->GetInCheck())
					{
						temp->w->SetInCheck(true);
						sheet->writeStr(row, 35, send);
					}
					else if (!temp->w->GetOutCheck())
					{
						temp->w->SetOutCheck(true);
						sheet->writeStr(row, 36, send);
					}
				}
				temp = temp->next;
				row++;
			}
			cout << "멈추려면 0 입력 >>";
			int stop = getche();
			if (stop == '0')
				break;
			else
				continue;
		}
	}
};

class Init
{
private:
	int work_min;							//매장을 지키는 최소 직원의 수
	int open_hour, close_hour;				//개/폐장 시간
	LinkedList *l = new LinkedList();
	bool madeTable = false;
public:
	void ShowMenu()
	{
		cout << "1. 정보 불러오기" << endl;
		cout << "2. 정보 저장하기" << endl;
		cout << "3. 가게 설정" << endl;
		cout << "4. 직원 생성" << endl;
		cout << "5. 직원 삭제" << endl;
		cout << "6. 직원 리스트 출력" << endl;
		cout << "7. 직원 스케줄표 만들기" << endl;
		cout << "8. 직원 스케줄표 오류 확인" << endl;
		cout << "9. 직원 스케줄표 수정" << endl;
		cout << "10. 출근부 출력" << endl;
		cout << "11. 출근체크 시작" << endl;
		cout << "12. 출퇴근 시간표 출력" << endl;
		cout << "13. 종료" << endl;
	}
	void StoreInfo()
	{
		l->StoreInfo();
		cout << "Info.txt에 데이터 저장 완료!" << endl;
	}
	void LoadInfo()
	{
		l->LoadInfo();
	}
	void SetStoreInfo()
	{
		cout << "매장을 지키는 최소 직원의 수를 입력하십시오 : ";
		cin >> work_min;
		cout << "개장 시간(24hr 기준으로 시간만 정수로 입력) : ";
		cin >> open_hour;
		cout << "폐장 시간(24hr 기준으로 시간만 정수로 입력) : ";
		cin >> close_hour;
	}
	void CreateWorker()
	{
		char name[20], position;
		int working_time, in_hour, out_hour, readResult;
		Worker *w = new Worker();
		Serial *a = new Serial("\\\.\\COM9");
		if (a->IsConnected())
			cout << "카드 정보를 입력받습니다. 카드를 리더기에 가져다 주세요." << endl;
		else
		{
			cout << "카드 리더기에 연결되지 않았습니다. 다시 시도해 주세요" << endl;
			return;
		}
		char incomingData[256] = "";
		while (!strcmp(incomingData, ""))
		{
			readResult = a->ReadData(incomingData, 256);
		}
		
		incomingData[readResult] = 0;

		cout << "등록된 카드번호는 " << incomingData << "입니다" << endl;
		w->SetCardInfo(incomingData);

		cout << "이름 입력 : ";
		cin >> name;
		w->SetName(name);
		cout << "직급 입력(1. 사장, 2. 점장, 3. 직원, 4. 알바) : ";
		cin >> position;
		w->SetPosition(position);
		cout << "주당 근로 시간 입력 : ";
		cin >> working_time;
		w->SetWorkingTime(working_time);
		cout << "출근시간 입력 : ";
		cin >> in_hour;
		cout << "퇴근시간 입력 : ";
		cin >> out_hour;
		w->SetWorkTime(in_hour, out_hour);

		l->insertNode(w);
		delete a;
		system("cls");
	}
	void DeleteWorker()
	{
		char name[20];
		cout << "삭제할 직원의 이름을 입력해 주세요 : ";
		cin >> name;
		ListNode *tmp = l->SearchNode(name);
		l->DeleteNode(tmp);
		system("cls");
		l->PrintList();
		cout << endl;
	}
	void MakeSchedule()
	{
		char name[20];
		cout << "스케줄을 생성할 직원의 이름을 입력하세요 : ";
		cin >> name;

		ListNode *worker = l->SearchNode(name);
		if (worker == NULL)
		{
			cerr << "잘못된 이름입니다." << endl;
			return;
		}
		worker->w->SetSchedule();
	}
	bool CheckError()
	{
		bool test = l->CheckError(work_min, open_hour, close_hour);
		return test;
	}
	void ModifySchedule()
	{
		char name[20];
		int menu;

		cout << "스케줄을 수정할 직원의 이름을 입력하세요 : ";
		cin >> name;

		ListNode *worker = l->SearchNode(name);
		if (worker == NULL)
		{
			cout << "찾는 직원은 리스트에 없습니다. 다시 확인하세요" << endl;
			return;
		}

		cout << "수정할 스케줄을 선택하세요. 1. 출/퇴근시간 2. 스케줄" << endl;
		cin >> menu;
		switch (menu)
		{
		case 1:
			int in, out;
			cout << "바꿀 출근시간을 입력하세요 : ";
			cin >> in;
			cout << "바꿀 퇴근시간을 입력하세요 : ";
			cin >> out;
			worker->w->SetWorkTime(in, out);
			cout << "변경 완료!" << endl;
			break;
		case 2:
			int day, input;
			bool work;
			do
			{
				cout << "바꿀 날짜를 입력하세요 : ";
				cin >> day;
				if (!(day >= 0 && day <= 31))
					cout << "잘못 입력하셨습니다. 다시 입력하세요." << endl;
			} while (day >= 0 && day <= 31);
			cout << "출근하려면 1, 아니면 0을 입력하세요 >>";
			cin >> input;
			if (input == 0)
				work = false;
			else
				work = true;
			worker->w->SetSchedule(day, work);
			cout << "변경 완료!" << endl;
			break;
		}
	}
	void PrintList()
	{
		l->PrintList();
	}
	void MakeTimeTable()
	{
		l->MakeTimeTable();
		madeTable = true;
	}
	void StartCheck()
	{
		if (madeTable == false)
			cerr << "출근부가 만들어지지 않았습니다. 출근부 먼저 만들어야 합니다." << endl;
		else
			l->StartCheck();
	}
};

int main()
{
	int menu;
	Init *i = new Init;
	bool setStore = false;
	bool test = false;

	while (1)
	{
		i->ShowMenu();
		cout << "메뉴 번호 입력 : ";
		cin >> menu;
		system("cls");
		switch (menu)
		{
		case 1:															//정보 불러오기
			i->LoadInfo();
			break;
		case 2:															//정보 저장하기
			i->StoreInfo();
			break;
		case 3:															//가게 정보입력
			i->SetStoreInfo();
			system("cls");
			setStore = true;
			break;
		case 4:
			i->CreateWorker();											//직원 생성
			break;
		case 5:
			i->DeleteWorker();											//직원 삭제
			break;
		case 6:															//직원 리스트 출력
			i->PrintList();
			break;
		case 7:															//직원 스케줄표 만들기
			i->MakeSchedule();
			break;
		case 8:															//직원 스케줄표 오류 확인
			test = i->CheckError();
			break;
		case 9:															//직원 스케줄표 수정
			i->ModifySchedule();
			break;
		case 10:															//출근부 만들기
			if (setStore == false)
				cerr << "가게 설정을 하지 않았습니다." << endl;
			else if (test == false)
				cerr << "오류 확인을 하지 않았습니다." << endl;
			else
				i->MakeTimeTable();
			break;
		case 11:															//출근체크 시작
			i->StartCheck();
			break;
		case 12:															//출퇴근 시간표 출력
			break;
		case 13:
			cout << "프로그램을 종료합니다." << endl;
			return 0;

		default:
			cout << "잘못 입력하셨습니다. 다시 입력하세요." << endl;
		}
	}

	return 0;
}