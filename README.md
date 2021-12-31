 <h1>Team Smart Hand Presentation</h1>


# 목차

1. 개요<br>
1.1 Smart Hand Presentation
2. Finger Module
<br>2.1 Intel Realsense D435
<br>2.2 Segmentation
3. HandWriting & Gesture Recognition
<br> 3.1 HandWriting Recognition
<br>3.2        Gesture Recognition
4. UI(User Interface)
<br>4.1       UI 생김새
<br>4.2        기본 키 설명
<br>4.3        글자 쓰기 모드
5. Result
<br>5.1        Delay
<br>5.2        Accuracy
<br>5.3        Resource
6. Reference


<h2>1. 개요

 <br>
 1.1 Smart Hand Presentation
</h2>

Presentation은 다른 사람에게 자신의 생각을 전달하는 행위라고 할 수 있다.

하지만 그런 중요한 일을 하는 와중에 흐름이 끊기는 것은 치명적이다.

Presentation을 하다가 프로젝터에 연결된 컴퓨터에 가서 조작을 하고 다시 Presentation을 하는 것을 반복하는 행위는 발표자 뿐만 아니라 청취자에게도 혼란스러움을 주기 마련이다.

이러한 현상을 막기 위해 현대에는 다음과 같은 보조 수단이 존재하지만

1. Smart Screen : 높은 가격 , 어려운 설치
2. Laser Pointer : 이벤트가 한정적
3. Tablet : 크기가 큰 경우 휴대성에 제한 , 크기가 작은 경우 활동성에 제한

과 같은 단점은 존재하기 마련이다.

이러한 단점들을 해소하기 위해 제안된 것이 바로 해당 **Smart Hand Presentation** 프로젝트이다.

이 프로젝트는 카메라를 이용하여 사용자로 하여금 아무런 준비물 없이도 손만으로 편리하게Presentation을 진행할 수 있도록 한다.

이 프로젝트를 수행함으로써 Presentation의 기존 보조 수단들의 장점들은 살리면서

단점들은 해소하는 결과를 얻을 수 있을 것이다.

이를 구현하기 위해 해당 프로젝트가 완성되었다고 가정하고 역으로 구현이 필요한 요소들을 명세한다.

크게 3가지 Module이 필요하다.

1. Finger Module
  A. 손으로 Presentation을 하기 위해선 강인한 손 인식이 필요
  B. 강인한 손 인식을 위해선 Frame에서 손을 추출해내는 기술이 필요
  C. 안정적으로 손 검출을 위해선 깔끔한Frame이 필요 ( Frame Processing )
  
2. Gesture Module
  A. Presentation을 조작하기 위해서는 Gesture 인식이 필요
  B. Dynamic Gesture 인식을 위해서는 Deep Learning이 필요
  
3. UI Module
  A. 사용자가 글씨를 쓰기 위해서는 PPT를 덮는 투명한 UI가 필요
  B. 손 글씨는 부정확할 수 있으므로 글자 보정이 필요
  C. 손 글씨를 인식하는 Deep Learning이 필요

다음과 같은 요구사항들을 바탕으로 구현에 착수한다.

<h2>2. Finger Module
<br>
2.1 Intel RealSense D435</h2>


### D435

![image](https://user-images.githubusercontent.com/96565110/147814981-b55cd574-12b9-4d29-ad43-36c34376813b.png)
![image](https://user-images.githubusercontent.com/96565110/147814987-795d4539-b643-4cee-8e31-01976ddbe5df.png)


### Frames

![image](https://user-images.githubusercontent.com/96565110/147814997-459c2e17-33c9-4ef7-80df-42e5615cafc5.png)


RGB Frame 과 Depth Frame을 동시에 Streaming &amp; Processing 가능

Depth Frame은 각 pixel에 대해 Depth Distance를 획득 할 수 있음



### Advantages of Depth Camera

해당 프로젝트의 핵심은 바로 이 Depth Camera이다.

기존의 RGB Camera 에서의 Vision 인식에는 치명적인 단점이 존재하고 ,

이 단점을 Depth Camera가 해결 해 줄 수 있기 때문이다.

RGB Camera의 단점은 다음과 같다.

첫째로는 조명에 매우 민감하다는 것 ,

둘째로는 사람의 피부색에 따라서 일정한 인식을 수행하지 못하는 것이다.

![image](https://user-images.githubusercontent.com/96565110/147815010-bc206ac2-d13f-47c1-af45-faffa8e8a401.png)
![image](https://user-images.githubusercontent.com/96565110/147815013-adc3139d-ec80-4063-a981-feb92712922c.png)


( 이를 보여주는 단적인 예시 , 조명이 없어지자 마자 다른 객체를 탐지함을 확인할 수 있다. )

![image](https://user-images.githubusercontent.com/96565110/147815016-0ad995e8-808d-4194-b0df-9a0e80d9ab0f.png)

반면 Depth Camera에서 얻을 수 있는 Depth Frame은 오로지 Distance data로만 사물을 판별하기 때문에

이러한 단점들을 Depth camera는 간단히 보완해줄 수 있다


### Intel RealSense SDK 2.0

![image](https://user-images.githubusercontent.com/96565110/147815215-35497218-364d-46a5-b4d2-7d2db84adf71.png)


- D435의 Frame을 처리하기 위해 필수적으로 요구되는 Kit

- Application용도에 맞게 설정된 Camera Preset 제공

- Frame에 대해서는 Filtering 및 Post-processing을 제공

- 단 , 비교적 최근에 나온 카메라들인 D400 series , T265부터 적용되는 SDK. 기존의 SDK 1.0 버전에서 제공되던 다양한 Application들은 사용할 수 없다.

- 아직은 참고할 만한 수준인Sample들만 제공되어 있는 상황이기 때문에

이번 프로젝트에서SDK는 Frame을 처리하는 정도의 역할만 수행하였고

그 외에는OpenCV를 활용하여 구현하였다.


## 2.2 Segmentation

![KakaoTalk_20191216_173631965](https://user-images.githubusercontent.com/96565110/147816509-7672125e-0118-4724-84b8-d07937ae122e.gif)

- SDK에서 제공하는 Align 기법 적용하여 두 Frame을 알맞게 매칭하였음.



## 3.1 Handwriting Recognition

-
### EMNIST DataSet

EMNIST 라는 외부의 데이터셋을 사용하였다.

- EMNIST Letters: 145,600 characters. 26 balanced classes.

대,소문자가 섞인 145,600개의 알파벳 손 글씨 데이터이며 26개의 Class 로 구성되어 있다.

데이터셋의 Shape 는 124800 x 28 x 28 이며

![image](https://user-images.githubusercontent.com/96565110/147815491-e1cd2b93-5c7b-4491-9894-860a8c21d9f7.png)

각 문자는 아래와 같은 형상을 하고있다.

![image](https://user-images.githubusercontent.com/96565110/147815493-b48e89a1-e027-414e-b558-00f99639a8ff.png)


### Handwriting Predict 및 추천 시스템의 구현

테스트를 위한 아래 GUI 의 흰색 부분에 글씨를 쓰면

![image](https://user-images.githubusercontent.com/96565110/147815522-d08f1c17-4a85-4cc7-9b78-04187c1e64ab.png)

위의 이미지를 모델의 Input 에 맞게 처리를 하며, 이를 Model에 집어넣어 Predict를 하면 다음과 같은 값들이 나오게 된다.

![image](https://user-images.githubusercontent.com/96565110/147815527-e6b02973-23e3-4a38-9f00-aef387e1b28f.png)

총 27 개의 인덱스, 0번째이후 1 ~ 26 까지가 각각 A~Z 이며 값이 가장 큰 것 일수록 해당 글자일 확률이 높다. 오차를 줄이기위해 정확도순 0~4 순위까지 출력하여 사용자가 선택할 수 있도록 하였다.( 4. UI 부분 참조)

## 3.2 Gesture Recognition

-
### Gesture Dataset

초기의 목표와 다르게 외부의 데이터셋 사용이 아닌 직접 촬영하여 데이터셋을 만들어 사용하였으며, 그 이유와 어려웠던 점에 대해서는 3.2 마지막 부분을 참조.

Class 는 총 5개이며, 각 Class 의 데이터 하나의 예시는 다음과 같다.

![image](https://user-images.githubusercontent.com/96565110/147815537-1d606013-0bd0-402a-bbeb-fc56a0c1a2e2.png)


0. Left

![image](https://user-images.githubusercontent.com/96565110/147815542-c3231036-79d3-4078-b07b-a16177b5e4e2.png)


1. Right

![image](https://user-images.githubusercontent.com/96565110/147815551-aa7052be-1846-4dd2-ab0e-a63ed8106b31.png)

2. Scroll

![image](https://user-images.githubusercontent.com/96565110/147815561-860c2081-ab34-42df-a897-be7111dba530.png)


3. Clap

![image](https://user-images.githubusercontent.com/96565110/147815570-27955373-bb5d-48cb-876a-d55ec2d7f586.png)


4. Idle

![image](https://user-images.githubusercontent.com/96565110/147815576-8007b077-1751-4a05-b690-507043f65359.png)


# UI (User Interface)

## 4.1 UI 생김새

![image](https://user-images.githubusercontent.com/96565110/147815666-f415d96c-042a-49d4-b4ba-955c67d9918a.png)


실제 구동시에는 뒤의 바탕화면이 투명하게 보이지만 이번 보고서에서는 바탕화면과의 차이를 알 수 있게 하기 위해서 투명도를 낮췄다.

## 4.2기본 키 설명

![image](https://user-images.githubusercontent.com/96565110/147815673-5be85c10-9574-4757-943a-73ffbcd5cf45.png)

-
### 1~5번 키

보고서 3.1 부분의 추천 시스템을 이용. 판에 그린 글자를 Deep Learning을 통해 얻은 가장 가까운 결과값 1~5까지를 각각의 버튼에 저장한다.

![image](https://user-images.githubusercontent.com/96565110/147815684-ac8a9a07-ed21-4e74-90e3-13b3aa472b97.png)
![image](https://user-images.githubusercontent.com/96565110/147815685-d2db3f3e-1369-4876-88d5-dbd2beefd132.png)
![image](https://user-images.githubusercontent.com/96565110/147815688-2208e4a7-8b94-4555-9fdb-ed83502eb660.png)

알파벳 A를 쓰는 과정동안 계속해서 1~5번의 키 값이 바뀌는 것을 확인 할 수 가 있다.

-
### Space

손 글씨를 실제로 화면으로 옮기는 도중 space 값 만큼은 만들 수가 없다는 것을 확인. 키 값 중에 하나를 space로 고정 시킨 뒤 공백을 두게 만들었다.

-
### Enter

실제 워드를 칠때처럼 Enter키를 칠 경우에 알파벳의 가장 첫 부분의 바로 밑의 줄로 이동. 그 후 다시 글자를 쓸 수 있도록 만들었다.

-
### C/L

Caps Lock의 기능을 하는 키. 한 번 누를 시 대문자가 소문자로 바뀌며 두 번 누를 시 소문자가 다시 대문자로 바뀐다.

-
### Clear

글씨를 쓰는 도중에 실수로 이상한 글자를 썼을 시 글자 칸을 다시 초기화 시켜준다.

-
### A/C

이전의 썼던 글자들을 지우는데 사용한다. 쓰다 보니 이전의 썼던 글자가 이상하다고 느낄 시 사용해서 글자를 지운다.

## 4.3글자 쓰기 모드

![image](https://user-images.githubusercontent.com/96565110/147815694-6559ae25-2936-4347-89ce-4a5dcf5fb78b.png)
![image](https://user-images.githubusercontent.com/96565110/147815696-42e2269d-48e4-42f2-bd6d-adf88f617e76.png)

![image](https://user-images.githubusercontent.com/96565110/147815704-88dec845-39d4-4a9a-add4-c3abf5608fdd.png)
![image](https://user-images.githubusercontent.com/96565110/147815710-a4480614-c423-4103-8983-b3f1bf4bf556.png)

![image](https://user-images.githubusercontent.com/96565110/147815718-2701c028-e658-4f79-b948-0a0d4fe6cf76.png)
![image](https://user-images.githubusercontent.com/96565110/147815720-2ce5e1b6-fc5b-4ca7-a924-6d18dc4720cc.png)


실제로 손 글씨를 그린 뒤 위의 1~5번 키를 통해 값을 입력하면 옆의 있는 화면에 그에 맞는 글자가 투영된다.

위의 결과 값은 HELO C WORLD 의 결과 값이다.

# Result

## 5.1Delay

- Finger recognition Delay : 0.01 sec 미만.
- Gesture recognition Delay : 최소 0.3 sec ~ 최대 3sec.
- HandWriting recognition Delay : 최초의 경우 1sec 이후 0.1 sec 미만.

## 5.2Accuracy

- Finger recognition Accuracy : 93%

( 1000 Frame에서 약 30 Frame 정도 오인식 )

- Gesture recognition Accuracy : 82%

( 제스처 인식에 사용되는 30Frame을 각 제스처마다 저장하여 1000번 테스트한 결과 )

- Hand Writing recognition Accuracy : 79%

( 실제 글자에 대해서는 94%를 상회하는 Accuracy를 보여주나 Finger로 쓴 글씨에 대해서는 정확도 하락. 다만 Recommend System을 도입하여 부족한 정확도를 보완 )

## 5.3Resource

실행환경 : CPU - Intel i5-8250U // RAM - 8GB

Finger Module + Gesture Module + UI Module 통합실행 시 ,

- CPU : 60% ~ 70% 점유
- Memory : 약500MB 점유

1.
# Reference

- Segmentation , recognition using Depth camera

[https://larrylisky.com/2016/11/26/gesture-recognition-from-depth-camera/](https://larrylisky.com/2016/11/26/gesture-recognition-from-depth-camera/)

-Hand tracking

[https://link.springer.com/article/10.1007/s11042-013-1501-1](https://link.springer.com/article/10.1007/s11042-013-1501-1)

-Simple Hand Gesture Recognition using OpenCV

[https://medium.com/@muehler.v/simple-hand-gesture-recognition-using-opencv-and-javascript-eb3d6ced28a0](https://medium.com/@muehler.v/simple-hand-gesture-recognition-using-opencv-and-javascript-eb3d6ced28a0)

-Real-time Hand Gesture Recognition with 3D CNNs

[https://github.com/ahmetgunduz/Real-time-GesRec](https://github.com/ahmetgunduz/Real-time-GesRec)

-Depth Post-processing for Intel Realsense D400 Depth Cameras

[https://www.mouser.com/pdfdocs/Intel-RealSense-Depth-PostProcess.pdf](https://www.mouser.com/pdfdocs/Intel-RealSense-Depth-PostProcess.pdf)

-The 20BN-jester Dataset V1

[https://20bn.com/datasets/jester](https://20bn.com/datasets/jester)

-nvGesture

[https://research.nvidia.com/publication/online-detection-and-classification-dynamic-hand-gestures-recurrent-3d-convolutional](https://research.nvidia.com/publication/online-detection-and-classification-dynamic-hand-gestures-recurrent-3d-convolutional)

-EMNIST Dataset

[https://www.nist.gov/node/1298471/emnist-dataset](https://www.nist.gov/node/1298471/emnist-dataset)

-tensorflow-Hangul-recognition

[https://github.com/IBM/tensorflow-hangul-recognitionl](https://github.com/IBM/tensorflow-hangul-recognitionl)

-convexHull Finger Detect

[https://www.researchgate.net/publication/280112085\_Obtaining\_Hand\_Gesture\_Parameters\_using\_Image\_Processing](https://www.researchgate.net/publication/280112085_Obtaining_Hand_Gesture_Parameters_using_Image_Processing)

-K-curvature Finger Detect

[https://www.researchgate.net/publication/323557218\_Real-Time\_Static\_and\_Dynamic\_Hand\_Gesture\_Recognition](https://www.researchgate.net/publication/323557218_Real-Time_Static_and_Dynamic_Hand_Gesture_Recognition)
