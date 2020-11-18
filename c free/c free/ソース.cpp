#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <conio.h>
#include <direct.h>
#include <io.h>
#include <float.h>

#pragma region DefineDeclaration
#define SAVEDIRECTORYPASS "H:\\TypingSaves"
#define TYPINGDIRECTORYPASS "H:TypingTxtFiles"

#define TYPINGFILEEXTENSION "txt"
#define SAVEFILEEXTENSION "bin"

#define LINESIZE 64
#define FILENAMESIZE 128
#define PLAYERNAMESIZE 64
#define TYPINGFILECOUNT 2
#define MAXTYPINGFILE 10
#pragma endregion

//�Z�[�u�t�@�C���̍\���̐錾
typedef struct {
	char filePass[FILENAMESIZE];
	double top10PlayerClearTime[11];
}TypingSave;

#pragma region PrototypeDeclaration
void TypingFanction(char[]);
void CountDown(int);
void ShuffleArray(char[][LINESIZE], int);
int CmpFanc(const void*, const void*);
void GetFilePass(char[], char[], char[], char[]);
void SelectTypingFileScene(char[][FILENAMESIZE], char[], char[], int);
TypingSave ReadTypingSave(char[]);
void WriteTypingSave(TypingSave, char[]);
int CountTxtFileLines(char[]);
void ReadTxtFile(char[][LINESIZE], int, char[]);
int GetFileNameOfDirectory(char[][FILENAMESIZE], char[]);
#pragma endregion 

int main(void) {
	char fileNames[MAXTYPINGFILE][FILENAMESIZE];
	char typingDirectoryPass[] = TYPINGDIRECTORYPASS;
	char typingFilePass[FILENAMESIZE];
	char saveFilePass[FILENAMESIZE];
	int fileCount;
	int lineCount;
	double clearTime;
	//�N���A�^�C�����X�V���ꂽ���i�t���O�j
	int isWriteClearTime = 0;
	//�^�C�s���O���͂������镶����z��|�C���^�i���I�m�ہj
	char(*words)[LINESIZE];
	TypingSave save;
	clock_t startClock, endClock;

	srand((unsigned)time(NULL));

	//�^�C�s���O�e�L�X�g�f�B���N�g�����̃t�@�C�����ƃt�@�C���p�X���擾
	fileCount = GetFileNameOfDirectory(fileNames, typingDirectoryPass);
	//�t�@�C���I�� & �Z�[�u�p�X�A���[�h�p�X�擾
	SelectTypingFileScene(fileNames, typingFilePass, saveFilePass, fileCount);
	//�Ώۂ̃^�C�s���O�e�L�X�g�t�@�C���̍s�����J�E���g
	lineCount = CountTxtFileLines(typingFilePass);

	//Debug
	//printf("%s\n%s\n", typingFilePass, saveFilePass);	//Debug : �p�X�擾�m�F

	//�z�񓮓I�m��
	words = (char(*)[LINESIZE])calloc(size_t(LINESIZE) * lineCount, sizeof(char));

	//�Ώۃp�X���當������擾���đ������
	ReadTxtFile(words, lineCount, typingFilePass);
	//������z����V���b�t��
	ShuffleArray(words, lineCount);

	printf(" %d�P��^�C�s���O\n", lineCount);

	if (words != NULL) {
		CountDown(3);
		startClock = clock();

		for (int i = 0; i < lineCount; i++)
			TypingFanction(words[i]);

		endClock = clock();
	}
	else {
		printf("�@������̓ǂݎ��Ɏ��s���܂���\n");
		exit(EXIT_FAILURE);
	}

	clearTime = (double)((LONGLONG)endClock - startClock) / CLOCKS_PER_SEC;
	printf("  \n �N���A�^�C��%.2lf �b\n", clearTime);

	save = ReadTypingSave(saveFilePass);
	save.top10PlayerClearTime[10] = clearTime;

	qsort(save.top10PlayerClearTime, _countof(save.top10PlayerClearTime), sizeof(save.top10PlayerClearTime[0]), CmpFanc);

	puts("\n-------------�����L���O--------------\n");
	for (int i = 0; i < 10; i++) {
		if (save.top10PlayerClearTime[i] != DBL_MAX) {
			printf(" %2d ��  %6.2lf �b", i + 1, save.top10PlayerClearTime[i]);
			if (save.top10PlayerClearTime[i] == clearTime && isWriteClearTime == 0) {
				if (i == 0) {
					printf("  ���V�L�^!!\n");
				}
				else {
					printf("�@�����Ȃ��̃��R�[�h\n");
				}
				isWriteClearTime = 1;
			}
			else
				putchar('\n');
		}
	}
	if (isWriteClearTime == 0)
		printf("\n              -�����N�O-              \n");
	puts("\n-------------------------------------\n");

	WriteTypingSave(save, saveFilePass);

	free(words);

	return 0;
}

void TypingFanction(char str[]) {
	int count = (unsigned)strlen(str);
	char tmp;

	for (int i = 0; i < count; i++) {
		printf("%s \r", &str[i]);
		fflush(stdout);
		while ((tmp = _getch()) != str[i]);
	}
}

void CountDown(int sec) {
	clock_t clock_start;
	clock_start = clock();
	do {
		printf("\r-%d-", sec - ((clock() - clock_start) / CLOCKS_PER_SEC));
	} while (((clock() - clock_start) / CLOCKS_PER_SEC) < sec);

	//��������
	printf("\r   \r");
}

void ShuffleArray(char words[][LINESIZE], int linesCount) {
	char tmp[LINESIZE];

	for (int i = linesCount; i > 0; i--) {
		int j = rand() % i;
		strcpy_s(tmp, sizeof(tmp), words[i - 1]);
		strcpy_s(words[i - 1], sizeof(words[i - 1]), words[j]);
		strcpy_s(words[j], sizeof(words[j]), tmp);
	}
}

int CmpFanc(const void* n1, const void* n2) {
	if (*(double*)n1 > * (double*)n2)
		return 1;
	else if (*(double*)n1 < *(double*)n2)
		return -1;
	else
		return 0;
}

void GetFilePass(char fileDirectory[], char fileName[], char filePass[], char extension[]) {
	char tmp[FILENAMESIZE];
	sprintf_s(tmp, FILENAMESIZE, "%s\\%s.%s", fileDirectory, fileName, extension);
	strcpy_s(filePass, FILENAMESIZE, tmp);
}

void SelectTypingFileScene(char fileNames[][FILENAMESIZE], char typingFilePass[], char saveFilePass[], int arrayCount) {
	int pAns;
	int isClear = 0;
	int tmp;
	char typingFileExtension[] = TYPINGFILEEXTENSION;
	char saveFileExtension[] = SAVEFILEEXTENSION;
	char saveDirectory[] = SAVEDIRECTORYPASS;
	char typingDirectory[] = TYPINGDIRECTORYPASS;

	while (1) {
		printf("-----�^�C�s���O���͂�I�����ĉ�����-----\n");
		for (int i = 0; i < arrayCount; i++)
			printf("%d : %s\n", i + 1, fileNames[i]);
		scanf_s("%d", &pAns);
		if (pAns < 1 || pAns > arrayCount) {
			printf("\n �͈͊O�̐��l�ł�\n");
			printf("�����L�[�������Ă�������...");
			tmp = _getch();
			system("cls");
		}
		else
			break;
	};
	GetFilePass(typingDirectory, fileNames[pAns - 1], typingFilePass, typingFileExtension);
	GetFilePass(saveDirectory, fileNames[pAns - 1], saveFilePass, saveFileExtension);
	system("cls");
	//Debug.Log
	//printf("%s\n%s\n", typingDirectory, saveDirectory); 
}

TypingSave ReadTypingSave(char filePass[]) {
	//�t�H���_���Ȃ��ꍇ
	if (_mkdir(SAVEDIRECTORYPASS) == 0)
		printf("\n �Z�[�u�t�H���_��V�K�쐬���܂���\n");

	FILE* fp = NULL;
	TypingSave save;

	for (int i = 0; i < 11; i++)
		save.top10PlayerClearTime[i] = DBL_MAX;

	fopen_s(&fp, filePass, "rb");

	if (fp != NULL) {
		fread(&save, sizeof(TypingSave), 1, fp);
		fclose(fp);

		return save;
	}
	else {
		fopen_s(&fp, filePass, "wb");
		if (fp != NULL) {
			printf("\n �Z�[�u�t�@�C����V�K�쐬���܂���\n");
			fclose(fp);
		}
		else
			printf("\n �Z�[�u�t�@�C���̐V�K�쐬�Ɏ��s���܂���\n");
	}

	return save;
}

void WriteTypingSave(TypingSave save, char filePass[]) {
	FILE* fp;
	fopen_s(&fp, filePass, "wb");

	if (fp != NULL) {
		fwrite(&save, sizeof(TypingSave), 1, fp);
		printf(" �����L���O���Z�[�u���܂���\n");
		fclose(fp);
	}
	else
		printf(" �����L���O�̃Z�[�u�Ɏ��s���܂���\n");

}

int CountTxtFileLines(char filePass[]) {
	errno_t err;
	FILE* fp;
	char tmp[256];
	int linesCount = 0;

	err = fopen_s(&fp, filePass, "r");

	if (fp != NULL) {
		while (fgets(tmp, (unsigned)_countof(tmp), fp) != NULL)
			if (tmp[0] != '\n')
				linesCount++;

		fclose(fp);
	}
	else {
		printf("File open failed\nerror function : CountTxtFileLines\n");
		printf("%s\n", filePass);
		exit(EXIT_FAILURE);
	}

	return linesCount;
}

void ReadTxtFile(char words[][LINESIZE], int linesCount, char filePass[]) {
	errno_t err;
	FILE* fp;

	err = fopen_s(&fp, filePass, "r");

	if (fp != NULL) {
		for (int i = 0; i < linesCount; i++) {
			fgets(words[i], LINESIZE, fp);
			//���s�݂̂Ȃ������Ȃ�
			if (words[i][0] == '\n') {
				i--;
				continue;
			}

			//���s������΍폜
			if (words[i][strlen(words[i]) - 1] == '\n')
				words[i][strlen(words[i]) - 1] = '\0';
		}

		fclose(fp);
	}
	else {
		printf("Open file failed\nerror fanction : ReadTxtFile\n");
		exit(EXIT_FAILURE);
	}
}

int GetFileNameOfDirectory(char fileNames[][FILENAMESIZE], char typingDirectoryPass[]) {

	char path[_MAX_PATH];
	char* ctx = NULL;
	struct _finddata_t fdata;
	intptr_t fh;
	int n = 1;
	int fileCount = 0;

	sprintf_s(path, _countof(path), "%s\\*", typingDirectoryPass);
	if ((fh = _findfirst(path, &fdata)) == -1) return -1;
	do {
		if ((fdata.attrib & _A_SUBDIR) == 0) {
			sprintf_s(path, _countof(path), "%s", fdata.name);
			n++;
			strcpy_s(fileNames[fileCount++], FILENAMESIZE, strtok_s(path, ".", &ctx));
			//Debug
			//printf("get file name of directory : %s\n", fileNames);	
		}
	} while (_findnext(fh, &fdata) == 0);
	_findclose(fh);

	return fileCount;
}