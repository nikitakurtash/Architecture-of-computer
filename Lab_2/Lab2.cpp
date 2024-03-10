#include <iostream>
#include <xmmintrin.h>
#include <mmintrin.h>
#include <immintrin.h>
#include <dvec.h>
#include <intrin.h>

//работаем с sse регистрами 64, 128, 256 бит
// (для 256 позволяет например сложить 8+8 интов за счет одной операции сложения - сокращает время, распаралелизовывая вычисления)

using std::cin;
using std::cout;
using std::endl;
using std::cerr;
using std::string;

__m64 _mm_add(__m64 m1, __m64 m2)
{
	__m64 result;
	__asm
	{
		movq mm0, m1
		movq mm1, m2
		paddsb mm0, mm1 //сложение упакованных байт со знаковым насыщением тк чар это 1 байт
		movq result, mm0
	}
	return result;
}

__m128 _mm_sub(__m128 m1, __m128 m2)
{
	__m128 result;
	__asm
	{
		movups xmm0, m1
		movups xmm1, m2
		subps xmm0, xmm1 //вычитание
		movups result, xmm0
	}
	return result;
}

__m256 _mm_div(__m256 m1, __m256 m2)
{
	__m256 result;
	__asm
	{
		vmovupd ymm0, m1
		vmovupd ymm1, m2
		vdivpd  ymm0, ymm0, ymm1 //деление
		vmovupd result, ymm0
	}
	return result;
}

int main(void) {
	int intReg1[2] = { 5, 5 }; //int32+int32 = 64
	int intReg2[2] = { 1, 9 }; //имеем два интовых регистра
	int intResult[2];

	_asm { //mmx
		movq mm0, intReg1 //кладем 64 бита в mm0 четверное слово(64 бит)
		movq mm1, intReg2
		paddd mm0, mm1 //векторное сложение регистра на регистр
		movq intResult, mm0 //результат в intResult
	}
	cout << "intReg1 + intReg2 =  ";
	for (int i = 0; i < 2; i++)
	{
		cout << intResult[i] << " ";
	} //выводим результат

	int ctrl = 0;
	__asm
	{
		finit
		fstcw ctrl
		and ctrl, 0xFFFE
		fldcw ctrl
	}

	float floatReg1[4] = { -3, 6, 3, 1 }; //для 128 бит
	float floatReg2[4] = { 7, 9, 2, 5 };
	float floatResult[4]; //резкльтат для 128 бит

	_asm { //sse
		movups xmm0, floatReg1 //перемещение невыровненных упакованных float
		movups xmm1, floatReg2
		mulps xmm0, xmm1 //векторное умножение float
		movups floatResult, xmm0
	}
	cout << endl << "floatReg1 * floatReg2 = ";
	for (int i = 0; i < 4; i++)
	{
		cout << floatResult[i] << " ";
	}

	double doubleReg[2] = { 16, 25 }; //для sse2 64 бита
	double doubleResult[2];

	_asm { //sse2 расширенный баор инструкций (берем корень)
		movups xmm0, doubleReg //перемещаем дабл
		sqrtpd xmm1, xmm0 //векторное вычисление корня 
		movups doubleResult, xmm1 //резкльтат
	}
	cout << endl << "sqrt(doubleReg) = " << doubleResult[0] << " " << doubleResult[1] << endl;

	float avxReg1[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };// 8 шт по 32 бита = 256 бит 
	float avxReg2[8] = { 5, 5, 5, 5, 5, 5, 5, 5 };
	float avxResult[8] = {};

	_asm {//avx
		vmovups ymm0, avxReg1 //аналог mov для avx(перед sse инструкциями просто ставим v)
		vmovups ymm1, avxReg2 //ymm регистр - 256 бит
		vsubps ymm3, ymm0, ymm1 //сюда точку останова - вычитание
		vmovups avxResult, ymm3
		vzeroall //очищает регистр avx
	}

	cout << "avxReg1 - avxReg2 = ";
	for (int i = 0; i < 8; ++i)
	{
		cout << avxResult[i] << ' ';
	}
	cout << endl;

	//dvec.h - векторы
	F64vec2 v1(1.0, 2.0); //вектор с двумя элементами ддабл
	F64vec2 v2(10.0, 11.0);
	v1 = v1 + v2;
	cout << "v1 + v2 = "; //по-хорошему показать дизассемблер нажимаем f10 или f11 каждый раз
	for (int i = 1; i >= 0; i--)
	{
		cout << v1[i] << " ";
	}
	cout << endl;

	//_mm_64 
	char mas_mmx_1[8] = { 0b1, 0b10, 0b100, 0b1000, 0b10000, 0b100000, 0b1000000, 0b1 }; //двоичные значения
	char mas_mmx_2[8] = { 0b11, 0b11, 0b11, 0b11, 0b11, 0b11, 0b11, 0b1111 };
	__m64 m64_1, m64_2;

	__asm {
		movq mm0, mas_mmx_1
		movq m64_1, mm0
		movq mm0, mas_mmx_2
		movq m64_2, mm0
	}
	m64_1 = _mm_add(m64_1, m64_2); //сложение
	__asm {
		movq mm0, m64_1
		movq mas_mmx_1, mm0
	}
	cout << "mas_mmx_1 + mas_mmx_2 = ";
	for (int i = 0; i < 8; ++i)
	{
		cout << mas_mmx_1[i] + 0 << ' ';
	}
	cout << endl;

	//_mm_128
	float mas_xmm_1[4] = { 1, 2, 3, 4 }; //аналогично для float
	float mas_xmm_2[4] = { 10, 5, 7, 4 };
	__m128 m128_1, m128_2;
	__asm {
		emms
		movups xmm0, mas_xmm_1
		movups m128_1, xmm0
		movups xmm0, mas_xmm_2
		movups m128_2, xmm0
	}
	m128_1 = _mm_sub(m128_1, m128_2);
	__asm {
		movups xmm0, m128_1
		movups mas_xmm_1, xmm0
	}
	cout << "mas_xmm_1 - mas_xmm_2 = ";
	for (int i = 0; i < 4; ++i)
	{
		cout << mas_xmm_1[i] << ' ';
	}
	cout << endl;

	//_mm_256 
	double mas_avx_1[4] = { 5, 8, 20, 9 }; //для дабл
	double mas_avx_2[4] = { 1, 4, 2, 3 };
	__m256 m256_1, m256_2;
	__asm {
		vmovupd ymm0, mas_avx_1
		vmovupd m256_1, ymm0
		vmovupd ymm0, mas_avx_2
		vmovupd m256_2, ymm0
	}
	m256_1 = _mm_div(m256_1, m256_2);
	__asm {
		vmovupd ymm0, m256_1
		vmovupd mas_avx_1, ymm0
	}
	cout << "mas_avx_1 / mas_avx_2 = ";
	for (int i = 0; i < 4; ++i)
	{
		cout << mas_avx_1[i] << ' ';
	}
	cout << endl;

	// Пример использования HADDPS
	float haddpsExample[4] = { 1.0, 2.0, 3.0, 4.0 }; // Исходные данные
	float haddpsResult[4]; // Для хранения результата

	__asm {
		movups xmm0, haddpsExample // Загрузка исходных данных в xmm0
		haddps xmm0, xmm0 // Выполнение горизонтального сложения
		movups haddpsResult, xmm0 // Сохранение результата
	}

	cout << "HADDPS result: ";
	for (int i = 0; i < 4; ++i) {
		cout << haddpsResult[i] << ' ';
	}
	cout << endl;

	// Пример использования HSUBPD
	double hsubpdExample[2] = { 3.0, 1.0 }; // Исходные данные
	double hsubpdResult[2]; // Для хранения результата

	__asm {
		movupd xmm0, hsubpdExample // Загрузка исходных данных в xmm0
		hsubpd xmm0, xmm0 // Выполнение горизонтального вычитания
		movupd hsubpdResult, xmm0 // Сохранение результата
	}

	cout << "HSUBPD result: ";
	for (int i = 0; i < 2; ++i) {
		cout << hsubpdResult[i] << ' ';
	}
	cout << endl;

	//intrin.h 

	double im256_1[12] = { 1,2,3,4,5,6,7,8,9,10,11,12 }; //чтобы быстрее сложить три массива быстро используем регистры avx и складываем по 4 три раза
	double im256_2[12] = { 1,1,1,2,2,2,3,3,3,4,4,4 };
	double im256_3[12]; //результат
	for (int i = 0; i < 12; i += 4) {

		__m256d x = _mm256_loadu_pd(&im256_1[i]); //грузим в х 256 бит по адресу i-го элемента первого массива (по 4 элем-та)
		__m256d y = _mm256_loadu_pd(&im256_2[i]);
		__m256d z = _mm256_add_pd(x, y); //сложили первые 4 дабла 256
		_mm256_store_pd(&im256_3[i], z); //сохранили сумму
	}
	cout << "im256_1 + im256_2 = ";
	for (int i = 0; i < 12; i++)
	{
		cout << im256_3[i] << " ";
	}
	__asm vzeroall; //чистим

	return 0;
}