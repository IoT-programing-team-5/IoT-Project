#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/input.h>

#define DOT_MATRIX_DEVICE "/dev/dot"
#define TACT_SWITCH_DEVICE "/dev/tactsw"

// 도트 매트릭스에 표시할 알파벳 배열
unsigned char alphabet[26][8] = {
    {0x18, 0x24, 0x42, 0x42, 0x7E, 0x42, 0x42, 0x42},   //A
                {0x3c, 0x22, 0x22, 0x3c, 0x22, 0x22, 0x22, 0x3c},   //B
                {0x1C, 0x22, 0x20, 0x20, 0x20, 0x20, 0x22, 0x1C},   //C
                {0x38, 0x44, 0x42, 0x42, 0x42, 0x42, 0x44, 0x38},   //D
                {0x3E, 0x20, 0x20, 0x3E, 0x20, 0x20, 0x20, 0x3E},   //E
                {0x3E, 0x20, 0x20, 0x3E, 0x20, 0x20, 0x20, 0x20},   //F
                {0x1C, 0x22, 0x42, 0x40, 0x40, 0x47, 0x42, 0x3C},   //G
                {0x42, 0x42, 0x42, 0x7E, 0x42, 0x42, 0x42, 0x42},   //H
                {0x1C, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x1C},   //I
                {0x1C, 0x08, 0x08, 0x08, 0x08, 0x48, 0x48, 0x30},   //J
                {0x44, 0x48, 0x50, 0x60, 0x50, 0x48, 0x44, 0x44},   //K
                {0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x3E},   //L
                {0x81, 0xC3, 0xA5, 0x99, 0x81, 0x81, 0x81, 0x81},   //M
                {0x42, 0x62, 0x52, 0x4A, 0x46, 0x42, 0x42, 0x42},   //N
                {0x3C, 0x42, 0x81, 0x81, 0x81, 0x81, 0x42, 0x3C},   //O
                {0x7C, 0x42, 0x42, 0x42, 0x7C, 0x40, 0x40, 0x40},   //P
                {0x38, 0x44, 0x82, 0x82, 0x82, 0x8A, 0x44, 0x3A},   //Q
                {0x7C, 0x42, 0x42, 0x42, 0x7C, 0x48, 0x44, 0x42},   //R
                {0x3C, 0x42, 0x40, 0x3C, 0x02, 0x02, 0x42, 0x3C},   //S
                {0x3E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08},   //T
                {0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3C},   //U
                {0x81, 0x42, 0x42, 0x42, 0x24, 0x24, 0x24, 0x18},   //V
                {0x81, 0x99, 0x99, 0x99, 0x99, 0x99, 0x5A, 0x24},   //W
                {0x81, 0x42, 0x24, 0x18, 0x18, 0x24, 0x42, 0x81},   //X
                {0x81, 0x81, 0x42, 0x42, 0x3C, 0x18, 0x18, 0x18},   //Y
                {0xFF, 0x03, 0x02, 0x04, 0x08, 0x10, 0x60, 0xFF}    //Z
};

// 택트 스위치 입력 처리
int readTactSwitch(int fd) {
    int ret, i;
    unsigned char push_sw_buff[8];
    ret = read(fd, &push_sw_buff, sizeof(push_sw_buff));
    if (ret < 0) {
        perror("Tact switch read error");
        return -1;
    }

    for (i = 0; i < 8; i++) {
        if (push_sw_buff[i] == 1) return i + 1; // 1-based index
    }
    return 0; // 아무 버튼도 눌리지 않았음
}


// 도트 매트릭스에 문자 표시
void displayOnDotMatrix(int fd, unsigned char* pattern) {
    if (write(fd, pattern, sizeof(alphabet[0])) < 0) {
        perror("Dot matrix write error");
        exit(1);
    }
}

int main(void) {
    int dot_fd, tact_fd, idx = 0, saved_idx = 0;

    // 도트 매트릭스 파일 열기
    dot_fd = open(DOT_MATRIX_DEVICE, O_RDWR);
    if (dot_fd < 0) {
        perror("Dot matrix device open error");
        return -1;
    }

    // 택트 스위치 파일 열기
    tact_fd = open(TACT_SWITCH_DEVICE, O_RDWR);
    if (tact_fd < 0) {
        perror("Tact switch device open error");
        close(dot_fd);
        return -1;
    }

    while (1) {
        int tact_index = readTactSwitch(tact_fd);
        switch(tact_index) {
            case 1: // Previous character
                idx = (idx - 1 + 26) % 26;
                break;
            case 2: // Save current character
                saved_idx = idx;
                printf("Saved character: %c\n", 'A' + idx);
                break;
            case 3: // Next character
                idx = (idx + 1) % 26;
                break;
        }
        if (tact_index > 0) {
            displayOnDotMatrix(dot_fd, alphabet[idx]); // 변경된 문자 표시
        }
        usleep(200000); // 0.2초 대기
    }

    close(dot_fd);
    close(tact_fd);
    return 0;
}
