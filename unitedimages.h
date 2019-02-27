#include "image.h"
#include "distance.h"
#include "stdio.h"
#include "maxflow/graph.h"
#include <iostream>
#include <fstream>
#include<map>
#pragma once


Image<Vec3b> produceLongImage(Image<Vec3b>& I, int willingLength);

Image<Vec3b> produceLongStructuredImage(Image<Vec3b>& I, int direction);