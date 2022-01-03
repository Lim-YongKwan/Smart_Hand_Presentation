/* Include the librealsense CPP header files */
#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include "../cv-helpers.hpp"
#include <opencv2/opencv.hpp>   // Include OpenCV API
#include <sstream>
#include <Windows.h>
#include <math.h>
#include <librealsense2/rs_advanced_mode.hpp>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <direct.h>


using namespace std;
using namespace rs2;
using namespace cv;

int main(int argc, char** argv) try
{
	rs2::colorizer color_map;
	// Obtain a list of devices currently present on the system
	context ctx;
	auto device = ctx.query_devices();
	auto dev = device[0];

	pipeline pipe;
	config cfg;
	string serial = dev.get_info(RS2_CAMERA_INFO_SERIAL_NUMBER);
	string json_file_name = "HandGesturePreset.json";

	cout << "Configuring camera : " << serial << endl;

	auto advanced_mode_dev = dev.as<rs400::advanced_mode>();

	// Check if advanced-mode is enabled to pass the custom config
	if (!advanced_mode_dev.is_enabled())
	{
		// If not, enable advanced-mode
		advanced_mode_dev.toggle_advanced_mode(true);
		cout << "Advanced mode enabled. " << endl;
	}

	rs2::align align_to_depth(RS2_STREAM_DEPTH);
	rs2::align align_to_color(RS2_STREAM_COLOR);

	std::ifstream t(json_file_name);
	std::string preset_json((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
	advanced_mode_dev.load_json(preset_json);
	cfg.enable_device(serial);
	cfg.enable_stream(RS2_STREAM_DEPTH, 640, 480, RS2_FORMAT_Z16);
	cfg.enable_stream(RS2_STREAM_COLOR);
	pipe.start(cfg);

	/*************/

	Ptr<BackgroundSubtractor> pBackSub = createBackgroundSubtractorMOG2();
	Mat frame, fgMask;
	Mat gray, blur_image, threshold_output;
	Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
	Mat kernel2 = getStructuringElement(MORPH_ELLIPSE, Size(10, 10));
	vector< pair<int, int> > black;
	float min_dep;
	int min_x, min_y;
	int min_roi_x, min_roi_y, max_roi_x, max_roi_y;
	Point prev_mouse;
	bool isfirst = true;
	int stop = 0;
	int capture_count = 0;
	int gesture_count = 0;
	bool capture_flag = false;

	/*************/

	while (true)
	{
		min_roi_x = 1000000, min_roi_y = 1000000, max_roi_x = -1, max_roi_y = -1;
		min_dep = 1000000;  min_x = 10000000; min_y = 10000000;

		rs2::frameset data = pipe.wait_for_frames();   // Wait for next set of frames from the camera
		frameset aligned_set = align_to_color.process(data);
		//auto image = frame_to_mat(aligned_set.get_color_frame());

		auto color_frame = aligned_set.get_color_frame();
		const int w_cf = color_frame.as<rs2::video_frame>().get_width();
		const int h_cf = color_frame.as<rs2::video_frame>().get_height();
		Mat image(Size(w_cf, h_cf), CV_8UC3, (void*)color_frame.get_data(), Mat::AUTO_STEP);
		Mat StartImage = image.clone();

		rs2::frame depth = aligned_set.get_depth_frame().apply_filter(color_map);
		rs2::depth_frame dep = aligned_set.get_depth_frame();
		// Get the depth frame's dimensions
		const int w = depth.as<rs2::video_frame>().get_width();
		const int h = depth.as<rs2::video_frame>().get_height();
		// Create OpenCV matrix of size (w,h) from the colorized depth data
		Mat depth_image(Size(w, h), CV_8UC3, (void*)depth.get_data(), Mat::AUTO_STEP);

		if (image.empty())
			break;
		//update the background model
		pBackSub->apply(image, fgMask);
		morphologyEx(fgMask, fgMask, CV_MOP_ERODE, kernel);
		morphologyEx(fgMask, fgMask, CV_MOP_DILATE, kernel2);

		/**************************************************/

		for (int i = 0; i < fgMask.rows; i++) //height
		{
			uchar* pointer_input = fgMask.ptr<uchar>(i);
			for (int j = 0; j < fgMask.cols; j++)	//width
			{
				if (pointer_input[j] == uchar(0))
				{

				}
				else if (pointer_input[j] == uchar(255))
				{
					if (j < min_roi_x) min_roi_x = j;
					if (j > max_roi_x) max_roi_x = j;
					if (i < min_roi_y) min_roi_y = i;
					if (i > max_roi_y) max_roi_y = i;
				}

			}
		}

		/**************************************************/

		for (int i = min_roi_y; i < max_roi_y; i++) //height
		{
			Vec3b* pointer_output = depth_image.ptr<Vec3b>(i);
			for (int j = min_roi_x; j < max_roi_x; j++)	//width
			{
				float dist_to_center = dep.get_distance(j, i);
				if (dist_to_center == 0.f) continue;
				if (dist_to_center < min_dep)
				{
					min_dep = dist_to_center;
					min_x = j; min_y = i;
				}
			}
		}
		/**************************************************/

		for (int i = 0; i < depth_image.rows; i++) //height
		{
			Vec3b* pointer_input = image.ptr<Vec3b>(i);
			Vec3b* pointer_output = depth_image.ptr<Vec3b>(i);
			for (int j = 0; j < depth_image.cols; j++)	//width
			{
				float dist_to_center = dep.get_distance(j, i);

				if (dist_to_center>min_dep + 0.08)
				{
					pointer_input[j] = Vec3b(0, 0, 0);
					pointer_output[j] = Vec3b(0, 0, 0);
				}
				else if (pointer_output[j] == Vec3b(0, 0, 0))
				{
					pointer_input[j] = Vec3b(0, 0, 0);
				}
			}
		}

		/**************************************************/

		Mat gray, blur_image, threshold_output, output;
		cvtColor(depth_image, gray, COLOR_BGR2GRAY);

		resize(gray, output, Size(160, 120));

		/**************************************************/

		float percent;
		if (capture_flag)
		{

			imwrite("./Gesture" + to_string(gesture_count - 1) + "/frame" + "_" + to_string(capture_count) + ".png", output);
			capture_count++;
		}

		imshow("gray", output);

		int keyboard = waitKey(30);
		if (keyboard == 'c')
		{
			capture_flag = true;
			cout << "Capture Start..." << endl;
			string g = "Gesture" + to_string(gesture_count);
			while (!(CreateDirectory(g.c_str(), NULL)))
			{
				gesture_count++;
				g = "Gesture" + to_string(gesture_count);
			}

			gesture_count++;
		}
		if (keyboard == 'v')
		{
			capture_flag = false;
			cout << "Capture Finish !" << endl;
			capture_count = 0;
		}
		if (keyboard == 'q' || keyboard == 27)
			break;
	}

	return EXIT_SUCCESS;
}
catch (const rs2::error & e)
{
	cerr << "RealSense error calling " << e.get_failed_function() << "(" << e.get_failed_args() << "):\n    " << e.what() << endl;
	return EXIT_FAILURE;
}
catch (const exception & e)
{
	cerr << e.what() << endl;
	return EXIT_FAILURE;
}