#include	<termio.h>

char
getChar(void)
{
	struct termio old_term, new_term;

	char	c;

	/* ���ߤ���������� */
	ioctl(0, TCGETA, &old_term);

	/* ����Υ��ԡ���Ĥ��� */
	new_term = old_term;

	/* ����ʸ���Υ��������޻ߤ����� */
	new_term.c_lflag &= ~(ICANON | ECHO);

	/* �������ϻߤ�ʤ���� */
	//new_term.c_lflag &= ~(ICANON);

	/* �����������ȿ�Ǥ��� */
	ioctl(0, TCSETAW, &new_term);

	/* 1 ʸ������ */
	c = getchar();

	/* �Ť�������᤹ */
	ioctl(0, TCSETAW, &old_term);

	return (c);
}	
