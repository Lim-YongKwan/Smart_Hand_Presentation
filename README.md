<h1>Smart Hand Presentation</h1>

<h2>                      목차<br><br>
          1.개요<br>
          1.1 Smart Hand Presentation<br>
          2 Finger Module<br>
          2.1 Intel Realsense D435
          <br>2.2	Frame Processing
          <br>2.3	Segmentation<br>
2.4	Finger Detection Algorithm<br>
2.5	Mouse Sync<br>
2.6	Finger Module Sequence<br>
3	Handwriting & Gesture Recognition
<br>3.1	Handwriting Recognition
<br>3.2	Gesture Recognition
<br>3.3	Gesture Recognition’s Difficulty
<br>4	UI (User Interface)
<br>4.1	UI 생김새
<br>4.2	기본 키 설명
<br>4.3	글자 쓰기 모드
<br>4.4	Sudo 코드
<br>5	Result
<br>5.1	Delay
<br>5.2	Accuracy	40
<br>5.3	Resource	40
<br>6	Reference	41
<br>
</h2>
1 개요<br>
1.1 Smart Hand Presentation<br>

Presentation은 다른 사람에게 자신의 생각을 전달하는 행위라고 할 수 있다.<br>
하지만 그런 중요한 일을 하는 와중에 흐름이 끊기는 것은 치명적이다.<br>

Presentation을 하다가 프로젝터에 연결된 컴퓨터에 가서 조작을 하고 다시 Presentation을 하는 것을 반복하는 행위는 발표자 뿐만 아니라 청취자에게도 혼란스러움을 주기 마련이다.<br>

이러한 현상을 막기 위해 현대에는 다음과 같은 보조 수단이 존재하지만<br>
1. Smart Screen  :  높은 가격 , 어려운 설치<br>
2. Laser Pointer  : 이벤트가 한정적<br>
3. Tablet  :  크기가 큰 경우 휴대성에 제한 , 크기가 작은 경우 활동성에 제한<br>

과 같은 단점은 존재하기 마련이다.<br>

이러한 단점들을 해소하기 위해 제안된 것이 바로 해당  “Smart Hand Presentation”  프로젝트이다.
<br>이 프로젝트는 카메라를 이용하여 사용자로 하여금 아무런 준비물 없이도 손만으로 편리하게 Presentation을 진행할 수 있도록 한다.<br>

이 프로젝트를 수행함으로써 Presentation의 기존 보조 수단들의 장점들은 살리면서
<br>단점들은 해소하는 결과를 얻을 수 있을 것이다.



이를 구현하기 위해 해당 프로젝트가 완성되었다고 가정하고 역으로 구현이 필요한 요소들을 명세한다.<br>
크게 3가지 Module이 필요하다.<br>
1. Finger Module<br>
<br>A. 손으로 Presentation을 하기 위해선 강인한 손 인식이 필요
<br>B. 강인한 손 인식을 위해선 Frame에서 손을 추출해내는 기술이 필요
<br>C. 안정적으로 손 검출을 위해선 깔끔한Frame이 필요 ( Frame Processing )
<br>2. Gesture Module
<br>A. Presentation을 조작하기 위해서는 Gesture 인식이 필요
<br>B. Dynamic Gesture 인식을 위해서는 Deep Learning이 필요
<br>3. UI Module
<br>A. 사용자가 글씨를 쓰기 위해서는 PPT를 덮는 투명한 UI가 필요
<br>B. 손 글씨는 부정확할 수 있으므로 글자 보정이 필요
<br>C. 손 글씨를 인식하는 Deep Learning이 필요
<br><br>
다음과 같은 요구사항들을 바탕으로 구현에 착수한다.
<br>2 Finger Module
<br>2.1 Intel RealSense D435
<br>• D435
![image](https://user-images.githubusercontent.com/96565110/147814104-babe71d7-0d10-4591-b7d9-a594f729ea65.png)
![image](https://user-images.githubusercontent.com/96565110/147814106-d7fe8ae2-a998-477f-9e18-98090d42bdd3.png)

• Frames
![image](https://user-images.githubusercontent.com/96565110/147814111-0b280241-807b-417f-a677-cf363e086d67.png)

<br>RGB Frame 과 Depth Frame을 동시에 Streaming & Processing 가능
<br>Depth Frame은 각 pixel에 대해 Depth Distance를 획득 할 수 있음
<br>• Advantages of Depth Camera
<br>해당 프로젝트의 핵심은 바로 이 Depth Camera이다.<br>
기존의  RGB  Camera 에서의  Vision  인식에는 치명적인  단점이 존재하고 ,
<br>이  단점을 Depth Camera가  해결 해 줄 수 있기 때문이다.
<br>RGB Camera의 단점은 다음과 같다.<br>
첫째로는 조명에 매우 민감하다는 것 , 
<br>둘째로는 사람의 피부색에 따라서 일정한 인식을 수행하지 못하는 것이다.
![image](https://user-images.githubusercontent.com/96565110/147814113-cff9c9e1-3394-4af3-ae6d-1d3dab329860.png)
![image](https://user-images.githubusercontent.com/96565110/147814116-4f7a9ac2-8278-4ef9-9809-8331be153ff1.png)

( 이를 보여주는 단적인 예시 ,  조명이 없어지자 마자 다른 객체를 탐지함을 확인할 수 있다. )

![image](https://user-images.githubusercontent.com/96565110/147814132-f9dd3e22-dbea-47a8-a975-9628fa4096f3.png)
<br>
반면 Depth Camera에서 얻을 수 있는 Depth Frame은 오로지 Distance data로만 사물을 판별하기 때문에<br>
이러한 단점들을 Depth camera는 간단히 보완해줄 수 있다

<br>
• H/W Limitations
프로젝트를 진행하는데 있어 제한사항이 존재하는지 사전에 체크.
<br>테스트 결과 , H/W적 한계가 존재함을 확인.
<br>
① Depth Distance
![image](https://user-images.githubusercontent.com/96565110/147814154-304e10bb-b8a3-4d5c-8a56-b4ce3450c104.png)

카메라 전방0.11m이내 Depth data의 손상 확인<br>
Depth Data 유효범위 : 0.11m ~ 10m<br>
② Optical Defects
![image](https://user-images.githubusercontent.com/96565110/147814164-ef8e0285-a209-4e75-8b0d-a99d7e4c80ab.png)


광원이 직접적으로 카메라를 비출 때 , 해당 물체에 대해 Depth data의 결손<br>


• Intel RealSense SDK 2.0



* D435의 Frame을 처리하기 위해 필수적으로 요구되는 Kit

* Application용도에 맞게 설정된 Camera Preset 제공

* Frame에 대해서는 Filtering 및 Post-processing을 제공

* 단 , 비교적 최근에 나온 카메라들인 D400 series , T265부터 적용되는 SDK. 기존의 SDK 1.0 버전에서 제공되던 다양한 Application들은 사용할 수 없다.

* 아직은 참고할 만한 수준인Sample들만 제공되어 있는 상황이기 때문에
이번 프로젝트에서SDK는 Frame을 처리하는 정도의 역할만 수행하였고
그 외에는OpenCV를 활용하여 구현하였다.


2.2 Frame Processing
• Intel SDK Post Processing
       
Depth Frame의 Noise를 개선하기 위해 SDK를 활용하여 후처리 기법 적용
* Decimation Filter : Intelligently reduces the resolution of a depth frame.
* Disparity : Performs transformation between depth and disparity domains.
* Spatial Filter : Applies edge-preserving smoothing of depth data.
* Temporal Filter : Filters depth data by looking into previous frames.
• Preset , Align
　　       
　　사전에 정의해둔 Camera Preset을 사용.
⇨ Depth Frame의 질 자체는 개선되었으나 RGB 와 Depth Frame을 매칭할 때,
이격이 발생하는 문제 발견.
⇨ SDK에서 제공하는 Align 기법 적용하여 두 Frame을 알맞게 매칭하였음.


2.3 Segmentation
Processing으로 얻은 안정적인 Frame에서 Hand를 분리해내는 작업 시작.
• BackgroundSubMOG2
Segmentation의 첫 작업은 배경과 전경을 분할하여 움직이는 객체만을 추출해내는 것이다.
이에 전경분할로 잘 알려진 OpenCV의 Background Subtractor알고리즘을 사용했다.

Background Subtractor 에도 다양한 구현방법이 존재하는데
각 픽셀에 적절한 수의 가우스 분포를 선택하여 , 환경변화에 더 잘 적응하는
Gaussian Mixture-based Segmentation Algorithm ( BackgroundSubMOG2 in OpenCV )
을 채택했다.

현재 프레임과 객체를 추출하기 위한 배경 모델의 차영상을 구하고 Threshold 처리하여 전경을 추출해낸다.

　



• Morphological Operation
　다만 이러한 분할 알고리즘을 사용하는 경우에 위 그림과 같이 많은 Noise가 생기게 되는데
　이를 해결하기 위해 Morphological Operation을 수행한다.
　
　이는 보통 Binary Image에서 흰색으로 표현된 오브젝트의 형태를 개선하기 위해 사용되는데
해당 프로젝트에서는 

* Erosion Operation : Binary Image에서 흰색 오브젝트의 외곽 픽셀을 검은색으로 만들어 노이즈를 제거하거나 붙어 있는 오브젝트들을 분리
* Dilation Operation : 흰색 오브젝트의 외곽 픽셀 주변에 흰색을 추가하여Erosion에 의해 작아졌던 오브젝트를 원래대로 돌려줌

이 두 연산을 차례로 수행하는 Opening기법을 사용하여 Noise를 개선하였다.
　
　


• Depth data
　해당 프로젝트는 유저의 손은 Camera에 가장 밀접한 움직이는 객체다 라는 전제조건이 존재한다. 
이 전제를 바탕으로 손을 추출하는 알고리즘을 구현하였다.

　4.1 에서 전경추출로 얻어낸 객체의 영역에 대해서만 Depth data를 획득한다.
그 중, 가장 가까운 거리에서부터 X cm 가까운 거리까지의 객체만을 남기고 모두 제거한다.
( 다수의 실험 결과 , 8 cm 로 설정하는 것이 가장 안정적임을 확인. )
　
• OpenCV Processing
　Frame을 2.2 에서 Processing 하였음에도 Noise가 존재함을 확인하였다.
　따라서 얻어낸 Frame을 Thresholding으로 Binary Image로 만든 후,
　Median filter & Interpolation 기법을 적용하여 비교적 깔끔한 depth 영상을 획득하는데 성공했다.
　        
2.4 Finger Detection Algorithm
Frame에서 Hand를 Segmentation한 결과를 획득하였으니
이제는 이를 바탕으로 Finger를 인식하는 알고리즘을 적용한다.
• convexHull Algorithm


get max contour : 가장 영역이 큰 윤곽선을 얻는다. 
get convex Hull : convex Hull 알고리즘으로 볼록 껍질 좌표들을 획득한다.
return valid points : 볼록 껍질 좌표들을 Finger로 판단하여 반환한다.
강인한 Finger Detection을 만족하지 못하였기 때문에 개선이 필요했다.


• K-curvature Algorithm
        
성능이 좋은 Finger Detection Algorithm으로 알려진 K-curvature Algorithm 으로 개선
get max contour : 가장 영역이 큰 윤곽선을 얻는다. 
get convex Hull : convex Hull 알고리즘으로 볼록 껍질 좌표들을 획득한다.
get convexity Defects : convexity Defects 알고리즘으로 볼록 결함 좌표들을 획득한다.
calculate distance & angle between convexity Defects : 볼록 결함 좌표 간의 거리 , 각도를 계산한다.
based distance & angle , return valid points : 계산결과를 바탕으로 알맞은 points를 반환
( 다수의 실험을 거친 결과 pixel distance > 20 , angle > 90 인 point를 반환하는 것이 가장 좋은 성능을 보임을 확인 )

⇨ 단순 convex Hull 알고리즘만으로 Detection 하는 것 보다 훨씬 강인한 Detection을 수행하는데 성공
　　
2.5 Mouse Sync
안정적인 손가락 위치를 획득하였다면 이를 바탕으로 Mouse와의 연동을 수행한다.
• Mouse Pointer
여러 손가락이 동시에 Mouse와 연동된다면 Mouse Position이 안정적이지 못하므로
단 하나의 손가락만 Mouse와 연동시킨다.
해당 프로젝트에서는 이 하나의 손가락을 5개의 손가락 중 ,  Hand Center에서 가장 먼 손가락으로 정의한다.
이를 구현하기 위해서는 먼저 Hand Center의 위치를 획득 할 필요가 있다.
• Hand Center
얻어낸 Hand 영상을 바탕으로 
Distance Transform 알고리즘을 사용하여 거리변환 행렬을 저장한다.
이 행렬은 Hand 영상에서의 각 하얀색 픽셀에서 가장 가까운 검은색 픽셀까지의 거리들을 의미한다.
즉 , 이 행렬에서 가장 거리가 큰 하얀색 pixel이 곧 Hand Center를 의미한다는 것을 알 수 있다.
이제 Hand Center에 해당하는 pixel을 얻기 위해서 minMaxIdx 알고리즘을 사용한다.
해당 알고리즘은 행렬에서 최소값과 최대값을 계산하고 각각의 위치를 배열에 저장해주는데
이를 통해 , 거리변환 행렬에서 가장 거리가 큰 pixel의 좌표와 값을 얻는다.
여기서 얻은 이 좌표가 바로 Hand Center이고 ,  값은 Hand Center를 중심으로 한 원의 반지름 이라고 할 수 있다.




• Smooth Mouse Move

* WIN32 API 를 이용하여 손가락의 위치로 Mouse Position을 이동시킨다.
       하지만 단순히 손가락의 위치로 Position을 Jump 시키는 방식은
       사용자에게 위화감을 줄 뿐더러 글씨쓰기와 같은 정확한 작업에는 부적합하다.

* 따라서 한번에 Jump하는 것이 아니라 이동할 Position으로 여러 번 단계를 나누어 이동하는 방식을 적용하였다.

* 약간의 Delay가 생성되지만 사용자가 체감 할 정도는 아니었고 대신에 부드러운 움직임을 제공해주기 때문에 단점보다는 장점이 더 크다는 판단 하에 이 부분은 무시하고 진행한다.

* 프로그램에서 가끔 사람의 손이 아닌 다른 부분을 인식하는 경우
　　Mouse Position이 갑자기 멀리 Kidnapping되는 현상 방지
　　
* 사람이 의도하지않은 움직임( ex>손떨림 )이 Mouse에는 반영되지 않도록 구현



2.6 Finger Module Sequence

Finger Program 작동절차


매 프레임마다 이러한 루틴을 반복
Finger Program은 Gesture Module, UI Module과의 결합에 중요한 역할을 수행한다.
총 4가지 Mode가 존재.
1. Zero Finger : UI Module 에서 Recognize 수행 , UI Mode가 아닐 시 , 왼쪽 클릭 수행.
2. One Finger : Pointer Mode On . Mouse 가 손가락을 추종하기 시작.
3. Two Finger : Paint Mode On. UI Module On. , 글자를 쓸 수 있음.
4. Five Finger : Gesture Mode On. UI Module Off. , 제스처 인식 시작.
3 Handwriting & Gesture Recognition
3.1 Handwriting Recognition
• EMNIST DataSet
EMNIST 라는 외부의 데이터셋을 사용하였다. ( Reference 참고 )
* EMNIST Letters: 145,600 characters. 26 balanced classes.
대,소문자가 섞인 145,600개의 알파벳 손 글씨 데이터이며 26개의 Class 로 구성되어 있다.
데이터셋의 Shape 는 124800 x 28 x 28 이며
 
각 문자는 아래와 같은 형상을 하고있다.




• Handwriting Learning Model

Learning Model 의 구조는 아래와 같이 되어있으며 간단하게 설명을 하면
model = keras.Sequential()
model.add(layers.Conv2D(32,kernel_size=(3,3), input_shape=(28,28,1),activation='relu'))
model.add(layers.Conv2D(64,(3,3),activation='relu')) 
# 2개의 Convolution 층
model.add(layers.MaxPooling2D(pool_size=8))
#가장 큰값을 추출하여 쓸데없는 값들을 버려준다. 8분의 1로 줄인다.
model.add(layers.Dropout(0.25))#과적합을 피하기위해 배치된 일부 노드들을 꺼준다 25%
model.add(layers.Flatten())#이미지를 다루기위해 2차원을 1차원으로 변환 
model.add(layers.Dense(512,activation='relu'))
model.add(layers.Dropout(0.4))
model.add(layers.Dense(27,activation='softmax')) 
# softmax 를 사용하여 Category 를 분류할수있게한다.
model.compile(loss=losses.categorical_crossentropy,optimizer="Adam",metrics=['accuracy'])
#loss 값을 categorical_crossentropy , optimizer은 Adam을 사용하며 측정항목으로 accuracy 를 사용
# Input 을 28x28x1 로하여 최종 Output 은 26가지의 경우로 나오게된다.



Model 의 Summary 는 다음과 같다


각 층의 Parameter을 조금씩 조정하며 테스트 하였으며
학습 과정에서Validation Set 에 대한 최종 Loss 값, Test Set에 대한 Accuracy 는


각각 0.15716 , 94.83%를 기록하였다.



• Handwriting Predict 및 추천 시스템의 구현
테스트를 위한 아래 GUI 의 흰색 부분에 글씨를 쓰면

위의 이미지를 모델의 Input 에 맞게 처리를 하며, 이를 Model에 집어넣어 Predict를 하면 다음과 같은 값들이 나오게 된다.

총 27 개의 인덱스, 0번째이후 1 ~ 26 까지가 각각 A~Z 이며 값이 가장 큰 것 일수록 해당 글자일 확률이 높다. 오차를 줄이기위해 정확도순 0~4 순위까지 출력하여 사용자가 선택할 수 있도록 하였다.( 4. UI 부분 참조)
3.2 Gesture Recognition
• Gesture Dataset
초기의 목표와 다르게 외부의 데이터셋 사용이 아닌 직접 촬영하여 데이터셋을 만들어 사용하였으며, 그 이유와 어려웠던 점에 대해서는 3.2 마지막 부분을 참조.

Class 는 총 5개이며, 각 Class 의 데이터 하나의 예시는 다음과 같다. 

0. Left 

1. Right



2. Scroll

3. Clap

4. Idle

시간 사정으로 인해 1개의 클래스는 한 사람의 것으로 이루어져 있으며(Clap)
Right , Left 같은 경우 외부 사람들의 도움을 받아 5명 정도의 다양성을 보이지만 그 외의 것들은 2 ~ 3명의 촬영 데이터로 이루어져 있다.
학습은 각 사진 파일들을 이름순으로 보았을 때 끝을 기준으로 10개를 사용한다.
각 클래스는 30~40 개의 데이터를 가지고 있으며, 총 데이터 개수는 188 개, 총 파일크기는 6.63MB 이다.
• Gesture Learning Model

Gesture Learning Model 은 Handwriting Learning Model 과 유사하며 다음과 같다.
# 각층의 설명은 Handwriting Learning Model 참조
model = keras.Sequential()
model.add(layers.Conv2D(64,kernel_size=(3,3), input_shape=InputSize,activation='relu'))
model.add(layers.Conv2D(128,(3,3),activation='relu'))
model.add(layers.MaxPooling2D(pool_size=4))
model.add(layers.Dropout(0.25))
model.add(layers.Flatten()) 
model.add(layers.Dense(256,activation='relu'))
model.add(layers.Dropout(0.4))
model.add(layers.Dense(ClassN,activation='softmax'))
model.compile(loss="categorical_crossentropy",optimizer="Adam",metrics=['accuracy'])

#Input Size 는 (30,45,1) Output 은 5


Model Summary 는 다음과 같고,


위의 구조로 모델을 10개 만들었으며 ( 이 이유는 Gesture Predict 란 참조)



169 개의 Train Set , 19개의 Validation Set 을 사용하여 각 모델의 최종 Val_Loss , Val_Accuracy는
모델 1 Val_Loss : 0.01229 Val_Accuracy : 100%
모델 2 Val_Loss : 0.00042 Val_Accuracy : 100%
모델 3 Val_Loss : 0.06161 Val_Accuracy : 100%
모델 4 Val_Loss : 0.00233 Val_Accuracy : 100%
모델 5 Val_Loss : 0.11154 Val_Accuracy : 94.74%
모델 6 Val_Loss : 0.18724 Val_Accuracy : 94.74%
모델 7 Val_Loss : 0.34985 Val_Accuracy : 84.21%
모델 8 Val_Loss : 0.21813 Val_Accuracy : 89.47%
모델 9 Val_Loss : 0.30930 Val_Accuracy : 89.47%
모델 10 Val_Loss : 0.55122 Val_Accuracy : 89.47%
이며 총 학습시간은 88.203 sec 소요


• Gesture Predict
먼저 Gesture Mode 에 들어가면 Finger 프로그램이 Depth 카메라를 통해 30 프레임을 저장하게 된다
30프레임 예시 파일


받은 데이터들을 각 모델에 순서대로 집어넣어 아래의 구조로 동작하여 각 프레임이 어느 동작인지 판단하여 클래스 값을 출력하게 된다.

30 프레임 예시 파일을 함수에 넣을 시 결과는 다음과 같이 나오게 된다.

처음 frame_0.png ~ frame_9.png 를 분석하고 실패 시 , 
frame_1.png ~ frame_10.png 를 분석하게 된다 …. frame_(n).png ~ frame_(n+9).png 
즉 하나씩 이동해가면서 분석하면서 클래스가 많이 나오고 정확도 또한 높은 구간을 실제 동작한 구간이라 판단하여 결과를 출력하도록 한 것이다.



클래스가 많이 나온 구간, 정확도에 따라 클래스를 판단하는 알고리즘을 만들었으며, 이에 대한 판단은 머신 러닝이 아닌 실제로 시연하였을 때 모델에서 나온 결과 값들을 관찰하면서 알고리즘을 만들었다.

위의 30프레임에 대하여 판정함수를 실행하였을 때 다음과 같은 결과를 확인할 수 있다.

frameProbN, frameProb, predRF 를 받아 
취합한 정확도(frameProb) 를 10으로(들어가는 프레임 10) 나누게 된다. 10으로 나누는 이유는 각 클래스가 정확도가 높지만 적게 나온경우, 정확도는 그리 높지 않지만 많이나온경우 둘 다 대응할수 있기 때문이다.


10으로 나눈 취합한 정확도에서 가장 큰 값을 찾아 받은 10프레임이 어떠한 클래스인지 찾게 된다. 그 이후에는 관찰을 통해 만든 알고리즘을 통해 클래스를 판단하여 return 1 ~ 4를 하게 된다.

Class 4 의 경우 Finger(C++) 프로그램에서 gesture 모드를 들어가는 동작이 

위 그림처럼 손바닥을 보여주는 모양이여서 Idle 클래스를 추가를 안했을 당시에 Gesture 모드 들어간 후 손바닥 모양을 다른 클래스로 오판단 하는 경우가 많았으며, 이 때문에 4번 Idle 클래스를 추가하여 ‘ 30 프레임 예시 파일’ 에서 4가 연속된 만큼 순차적으로 frame_0.png ~ frame_15.png 까지 뛰어넘어 frame_16.png 부터 분석하게 된다.


이 외의경우 간단하게 설명하면 마지막 분석 프레임이 frame_10.png 일 시 순서대로 분석한 것이 [1,4,4…..] 일 경우 첫번째가 1이므로 한칸 이동하여 frame_2 ~ frame_11 까지 분석한다 이 후 첫번째가 4가 아니므로 동일하게 한칸 이동, 마지막 프레임 frame_12 일때 4가 처음부터 연속으로 나왔으므로 쭉 frame 들을 건너뛰며 다음 frame 을 분석하게 된다.

Class 를 판단하게 되면, 판단한 결과를 소켓 통신을 통해
 
결과 Class 를 Finger(C++) 로 보내게 되며, 만일 프레임 파일들을 모두 읽었을 때 예상한 결과값이 안나온다면 

-1 값을 보내어 Finger(C++) 프로그램에서 30프레임을 다시받아와 분석하게 된다.


3.3 Gesture Recognition’s Difficulty
• Difference of Dataset’s Environment
프로젝트 초기에는 다음과 같은 외부 데이터셋을 이용하여 하려고 했으나
20BN-jester (83.4GB)

Nvgesture(30GB)


현 프로젝트에서 작동하는 환경과 달라 해당 Dataset으로 학습한다고 했을 때 정확한 결과값이 나오지가 않았다.

4 UI (User Interface)
4.1 UI 생김새


실제 구동시에는 뒤의 바탕화면이 투명하게 보이지만 이번 보고서에서는 바탕화면과의 차이를 알 수 있게 하기 위해서 투명도를 낮췄다.


4.2 기본 키 설명

• 1~5번 키
보고서 3.1 부분의 추천 시스템을 이용. 판에 그린 글자를 Deep Learning을 통해 얻은 가장 가까운 결과값 1~5까지를 각각의 버튼에 저장한다.


알파벳 A를 쓰는 과정동안 계속해서 1~5번의 키 값이 바뀌는 것을 확인 할 수 가 있다.



• Space
손 글씨를 실제로 화면으로 옮기는 도중 space 값 만큼은 만들 수가 없다는 것을 확인. 키 값 중에 하나를 space로 고정 시킨 뒤 공백을 두게 만들었다.
• Enter
실제 워드를 칠때처럼 Enter키를 칠 경우에 알파벳의 가장 첫 부분의 바로 밑의 줄로 이동. 그 후 다시 글자를 쓸 수 있도록 만들었다.
• C/L
Caps Lock의 기능을 하는 키. 한 번 누를 시 대문자가 소문자로 바뀌며 두 번 누를 시 소문자가 다시 대문자로 바뀐다.
• Clear
글씨를 쓰는 도중에 실수로 이상한 글자를 썼을 시 글자 칸을 다시 초기화 시켜준다.
• A/C
이전의 썼던 글자들을 지우는데 사용한다. 쓰다 보니 이전의 썼던 글자가 이상하다고 느낄 시 사용해서 글자를 지운다.


4.3 글자 쓰기 모드

 
 
 

실제로 손 글씨를 그린 뒤 위의 1~5번 키를 통해 값을 입력하면 옆의 있는 화면에 그에 맞는 글자가 투영된다.
위의 결과 값은 HELO C WORLD 의 결과 값이다. 


4.4 Sudo 코드
총 3가지의 클래스를 통하여 만들었다.
• Location 클래스

• Drawing Pad 클래스




• BackGround 클래스

5 Result
5.1 Delay
* Finger recognition Delay : 0.01 sec 미만.
* Gesture recognition Delay : 최소 0.3 sec ~ 최대 3sec.
* HandWriting recognition Delay : 최초의 경우 1sec 이후 0.1 sec 미만.
5.2 Accuracy
* Finger recognition Accuracy : 93% 
( 1000 Frame에서 약 30 Frame 정도 오인식 )
* Gesture recognition Accuracy : 82% 
( 제스처 인식에 사용되는 30Frame을 각 제스처마다 저장하여 1000번 테스트한 결과 )
* Hand Writing recognition Accuracy : 79% 
( 실제 글자에 대해서는 94%를 상회하는 Accuracy를 보여주나 Finger로 쓴 글씨에 대해서는 정확도 하락.  다만 Recommend System을 도입하여 부족한 정확도를 보완 )
5.3 Resource
실행환경 : CPU - Intel i5-8250U // RAM - 8GB
Finger Module + Gesture Module + UI Module 통합실행 시 ,
* CPU : 60% ~ 70% 점유
* Memory : 약500MB 점유

6 Reference

- Segmentation , recognition using Depth camera
https://larrylisky.com/2016/11/26/gesture-recognition-from-depth-camera/
-Hand tracking
https://link.springer.com/article/10.1007/s11042-013-1501-1
-Simple Hand Gesture Recognition using OpenCV
https://medium.com/@muehler.v/simple-hand-gesture-recognition-using-opencv-and-javascript-eb3d6ced28a0
-Real-time Hand Gesture Recognition with 3D CNNs
https://github.com/ahmetgunduz/Real-time-GesRec
-Depth Post-processing for Intel Realsense D400 Depth Cameras
https://www.mouser.com/pdfdocs/Intel-RealSense-Depth-PostProcess.pdf
-The 20BN-jester Dataset V1
https://20bn.com/datasets/jester
-nvGesture
https://research.nvidia.com/publication/online-detection-and-classification-dynamic-hand-gestures-recurrent-3d-convolutional
-EMNIST Dataset
https://www.nist.gov/node/1298471/emnist-dataset
-tensorflow-Hangul-recognition
https://github.com/IBM/tensorflow-hangul-recognitionl



-convexHull Finger Detect
https://www.researchgate.net/publication/280112085_Obtaining_Hand_Gesture_Parameters_using_Image_Processing
-K-curvature Finger Detect
https://www.researchgate.net/publication/323557218_Real-Time_Static_and_Dynamic_Hand_Gesture_Recognition

Page 11 / 17


