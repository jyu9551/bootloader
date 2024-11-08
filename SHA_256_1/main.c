#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "KISA_SHA256.h"
// https://seed.kisa.or.kr/kisa/Board/21/detailView.do
// CertUtil -hashfile SCU_Gen6_Training.bin SHA256

int main() {
    FILE *file;
    char *buffer;
    long file_size;

    // 파일 열기
    file = fopen("SCU_Gen6_Training.bin", "rb"); // "rb"는 이진 모드로 읽기를 나타냄

    if (file == NULL) {
        perror("file open fail");
        return 1;
    }

    // 파일 크기 확인
    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // 메모리 할당
    buffer = (char*)malloc(file_size);
    if (buffer == NULL) {
        perror("memory allocation fail");
        fclose(file);
        return 1;
    }

    // 파일 내용을 버퍼에 읽어오기
    fread(buffer, 1, file_size, file);

    unsigned char sha256Buffer[32] = {0,};

    SHA256_INFO info;
	SHA256_Init( &info );
    char key[32];
    (void)memset(key, 0x11, sizeof(key));
    SHA256_Process( &info, key, 32 );
	SHA256_Process( &info, buffer, file_size );
	SHA256_Close( &info, sha256Buffer );
    for(int i=0; i<sizeof(sha256Buffer); i++)
    {
        printf("%02x", sha256Buffer[i]);
    }
    printf("\n");

    // 파일 및 버퍼 닫기
    fclose(file);

    // 여기에서 버퍼를 사용할 수 있습니다

    // 메모리 해제
    free(buffer);

    system("pause");
    return 0;
}