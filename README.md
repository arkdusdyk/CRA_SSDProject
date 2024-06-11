## CRA SSDProject
---
#### 개요
팀프로젝트 컨셉에 맞게 SSD Project와 TestShell Project를 별도의 Visual Studio Solution으로 개발 (SSD, TestShell 각자 개발)

#### Solution 빌드 순서
TestShell -> SSD

### 프로그램 실행 준비
1. SSD, TestShell 솔루 빌드
2. 실행파일 있는 파일에 TestShell/TestShell 내에 있는 testcases, testsuites 폴더를 폴더채로 복사해서 붙이기

### 프로그램 실행
1) Test Shell 실행 : ./TestShell.exe

2) SSD 실행 : ./SSD.exe

3) Runner Test Script 단위로 수행 : ./TestShell.exe run_list.lst
- testsuites 내의 run_list.lst 읽어서 실행됨
- TC 추가하는 법? testcases 에 TC 추가 및 testsuites/run_list.lst 추가