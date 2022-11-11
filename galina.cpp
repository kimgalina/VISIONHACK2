
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

#include <opencv2/imgcodecs.hpp>
#include<opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp> // подключаем новую библиотеку !!!
#include<iostream>

using namespace cv;
using namespace std;


//////// CAR PLATES DETECTION ///////

/* Эта программа распознает лица в режиме реального времени и отслеживает их.
Для этого используются предварительно обученные XML-классификаторы.
Классификаторы, используемые в этой программе, имеют обученные черты лица.
Для обнаружения разных объектов можно использовать разные классификаторы.*/


/*Пути к XML файлам классификатора должны быть указаны до выполнения программы.
Эти файлы XML можно найти в каталоге OpenCV «opencv/data/haarcascades».*/

int main()
{	/////вместо пути нам нужен идентификационный номер нашей камеры , в нашем случае цифра 1 показфвает что у нас одна камера 
	// это будет подключено автоматически к любой камере подключенной к одной и той же сети WIFI  с данным компьютером 
	// если мы поставим ноль >>  мы не используем камеру 



	tesseract::TessBaseAPI* api = new tesseract::TessBaseAPI();
	// Initialize tesseract-ocr with English, without specifying tessdata path
	if (api->Init("C:/CODE/ocr/tessdata", "eng")) {
		fprintf(stderr, "Could not initialize tesseract.\n");
		exit(1);
	}

	///////////////////Webcameras//////////////
	VideoCapture cap(1);// спользуется класс VideoCapture() для создания объекта  cap , который затем будем использовать для чтения видеофайла
	Mat img;


	CascadeClassifier plateCascade;
	plateCascade.load("Resources/haarcascade_russian_plate_number.xml");

	if (plateCascade.empty()) {
		cout << " error" << endl;
	}

	vector<Rect>plates;




	while (true) {

		cap.read(img);//  читаем картинки с объекта cap 
		plateCascade.detectMultiScale(img, plates, 1.1, 10);//Обнаруживает объекты разного размера на входном изображении.
		//Обнаруженные объекты возвращаются в виде списка прямоугольников.plates - это список прямоугольников -  номеров 


		for (int i = 0; i < plates.size(); i++)
		{
			Mat imgCrop = img(plates[i]);
			//imshow(to_string(i), imgCrop);

			//функция std::to_string(), которая позволяет преобразовать передаваемое значение в строку. 
			// в функции imshow есть два аргумента первое переодически выводит новое имя - цифру , а второе саму картинку 


			rectangle(img, plates[i].tl(), plates[i].br(), Scalar(0, 0, 255), 3); // теперь мы рисуем на нашей картинке эти 
			// прямоугольники этих прямоунг столько же сколько лиц и обрисовывается прямоуг по левому вверхнему и нижнему правому углам




			string  path_to_ImgCrop = "Resources/car_plates/" + to_string(i) + ".png";// путь к нашей картинке , котоорый мы должгы передать  
			imwrite(path_to_ImgCrop, imgCrop);

			//Pix* image = pixRead("C:/CODE/ocr/test1.png");// читаем текст с картинки по ее пути 

			const char* path = path_to_ImgCrop.c_str(); // тк функция pixRead читает только значения const char* то переводим string в const char*


			Pix* image = pixRead(path);
			// Open input image with leptonica library

			char* outText;

			api->SetImage(image);
			// Get OCR result
			outText = api->GetUTF8Text();
			printf("OCR output:\n%s", outText);

			// освобождаем память 
			api->End();
			delete api;
			delete[] outText;
			pixDestroy(&image);


		}




		imshow("Image", img);// функция , показывает картинку
		//добавим задержку чтобы окно не закрылось сразу
		waitKey(1);
	}
	return 0;
}












