@echo off
rem -------------------------------------------------------------------------
rem 
rem *.h�t�@�C����*.cpp�t�@�C���̕����R�[�h��UTF-8(BOM�t)�ɕϊ�����o�b�`�t�@�C��
rem  VisualStudio�̂��߂ɍ����
rem  �g���ۂ�nkf32.exe���Q�Ƃł����Ԃɂ��Ă�������
rem  (������̊��ł�Windows/System32/�̒���nkf32.exe��u���Ă���)
rem  nkf32.exe�������킩��Ȃ���΁A"nkf"�Ō������Ă�������
rem 
rem -------------------------------------------------------------------------
IF EXIST nkf32 (GOTO  FILE_TRUE) ELSE GOTO FILE_FALSE

:FILE_TRUE
dir /s /b *.h > LLLLLLLLLLLLLL.txt
for /f %%a in (LLLLLLLLLLLLLL.txt) do (
    echo %%a
    nkf32 -w8 %%a > hoge.txt
    copy hoge.txt /b %%a
    del hoge.txt
)

dir /s /b *.cpp > LLLLLLLLLLLLLL.txt
for /f %%a in (LLLLLLLLLLLLLL.txt) do (
    echo %%a
    nkf32 -w8 %%a > hoge.txt
    copy hoge.txt /b %%a
    del hoge.txt
)

del LLLLLLLLLLLLLL.txt

GOTO FIN

:FILE_FALSE
echo nkf���Ȃ��ł�
pause

:FIN