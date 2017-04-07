#include "Bezier.h"



CBeizier::CBeizier() : list(nullptr), speed(0.f)
{
	//points.push_back({});

	easeInoutBack.reserve(MAX_POINTS);
	slowMiddle.reserve(MAX_POINTS);
	bLineal.reserve(MAX_POINTS);
	bshake.reserve(MAX_POINTS);
	bfly.reserve(MAX_POINTS);
	temp.reserve(MAX_POINTS);


	std::vector<fPoint> points;
	//cool efect; stop in the middle;
	points.push_back({ 0.95f,1.f });
	points.push_back({ 1.2f,-0.0f });
	Bezier(points, cbezier_type::CB_SLOW_MIDDLE);
	points.clear();
	//bot one
	/*points.push_back({ 0.6f, 0.1f});
	points.push_back({ 0.55f, 0.15f });
	points.push_back({ 0.4f, -0.4f });
	points.push_back({ 0.45f, -0.45f });
	points.push_back({ 0.85f, 1.f });
	points.push_back({ 0.95f, 1.f });
	*/
	Bezier(points, cbezier_type::CB_LINEAL);
	points.clear();
	//rebot dalt
	points.push_back({ 0.6f, 1.2f });
	points.push_back({ 0.55f, 1.25f });
	points.push_back({ 0.4f, 0.8f });
	points.push_back({ 0.45f, 0.85f });
	points.push_back({ 0.85f, 1.f });
	points.push_back({ 0.95f, 1.f });

	Bezier(points, cbezier_type::CB_EASE_INOUT_BACK);
	points.clear();
	
	points.push_back({0.08f,1.0f});
	points.push_back({ 0.16f,1.0f });
	points.push_back({ 0.24f,-1.0f });
	points.push_back({ 0.32f,-1.0f });
	points.push_back({ 0.40f,1.0f });
	points.push_back({ 0.48f,1.0f });
	points.push_back({ 0.56f,-1.0f });
	points.push_back({ 0.64f,-1.0f });
	points.push_back({ 0.72f,1.0f });
	points.push_back({ 0.80f,1.0f });
	points.push_back({ 0.88f,-1.0f });
	points.push_back({ 0.96f,-1.0f });
	
	Bezier(points, cbezier_type::CB_SHAKE,0);
	points.clear();

	points.push_back({ 0.3f,-0.4f });
	points.push_back({ 0.2f, 1.5f });
	Bezier(points, cbezier_type::CB_FLY);
	points.clear();

}


CBeizier::~CBeizier()
{
	easeInoutBack.clear();
	slowMiddle.clear();
	bLineal.clear();
	bshake.clear();
	temp.clear();
}

void CBeizier::DrawBezierCurve(cbezier_type type, iPoint position)
{
	int size = 0;
	switch (type)
	{
	case CB_EASE_INOUT_BACK:
	{
		//		for (int i = 0; i+1 < pivotPoints.size(); i++)
		//		{
		//			app->render->DrawLine(pivotPoints[i].x*200 + 300, -pivotPoints[i].y * 100 + 400, pivotPoints[i+1].x * 200 + 300, -pivotPoints[i+1].y * 100 + 400, 255, 255, 255, 155);
		//		}
		size = easeInoutBack.size();
		for (int i = 0; i + 1 < size; i++)
		{
			app->render->DrawLine((i * 2) + position.x, -(easeInoutBack[i] * 100) + position.y, (i * 2) + position.x, -(easeInoutBack[i + 1] * 100) + position.y, 255, 150, 0, 255);
		}
	}
	break;
	case CB_SLOW_MIDDLE:
	{
		size = slowMiddle.size();
		for (int i = 0; i + 1 < size; i++)
		{
			app->render->DrawLine((i * 2) + position.x, -(slowMiddle[i] * 100) + position.y, (i * 2) + position.x, -(slowMiddle[i + 1] * 100) + position.y, 255, 150, 0, 255);
		}
	}
	break;
	case CB_LINEAL:
	{
		size = bLineal.size();
		for (int i = 0; i + 1 < size; i++)
		{
			app->render->DrawLine((i * 2) + position.x, -(bLineal[i] * 100) + position.y, (i * 2) + position.x, -(bLineal[i + 1] * 100) + position.y, 255, 150, 0, 255);
		}
	}
	break;
	case CB_SHAKE: {
		size = bshake.size();
		for (int i = 0; i + 1 < size; i++)
		{
			app->render->DrawLine((i * 2) + position.x, -(bshake[i] * 100) + position.y, (i * 2) + position.x, -(bshake[i + 1] * 100) + position.y, 255, 150, 0, 255);
		}
	}
	break;
	case CB_FLY: {
		size = bfly.size();
		for (int i = 0; i + 1 < size; i++)
		{
			app->render->DrawLine((i * 2) + position.x, -(bfly[i] * 100) + position.y, (i * 2) + position.x, -(bfly[i + 1] * 100) + position.y, 255, 150, 0, 255);
		}
	}
	default:
		break;
	}
}

float CBeizier::GetActualPoint(const iPoint origin, const iPoint destination, int ms, int current_ms, cbezier_type b_type)
{
	float distance = -(sqrt(pow((destination.x - origin.x), 2) + pow((destination.y - origin.y), 2)));
	return (GetActualX(ms, current_ms, b_type)*distance);
}

float CBeizier::GetActualX(int ms, int current_ms, cbezier_type b_type)
{
	int time = (current_ms * 100 / ms);
	float ret = 0.0f;
	switch (b_type)
	{
	case CB_EASE_INOUT_BACK:
	{
		if (time >= easeInoutBack.size()) ret = easeInoutBack[99];
		else	ret = easeInoutBack[time];
		break;
	}
	case CB_SLOW_MIDDLE:
	{
		if (time >= slowMiddle.size())
		{
			ret = slowMiddle[99];
		}
		else	ret = slowMiddle[time];
		break;
	}	
	case CB_LINEAL:
	{
		if (time >= bLineal.size()) ret = bLineal[99];
		else	ret = bLineal[time];
		break;
	}
	case CB_SHAKE:
	{
		if (time >= bshake.size()) ret = bshake[99];
		else	ret = bshake[time];
		break;
	}
	case CB_FLY: {
		if (time >= bfly.size()) ret = bfly[99];
		else	ret = bfly[time];
		break;
	}
	default: ret = 0.0f;
		break;
	}
	return ret;
}

bool CBeizier::CalculatePivots(std::vector<fPoint> &points)
{
	//pushback origin
	pivotPoints.push_back({ 0.0f,0.0f });

	int size = points.size();
	for (int i = 0; i + 3 < size; i += 2)
	{
		pivotPoints.push_back(points[i]);
		pivotPoints.push_back(points[i + 1]);
		pivotPoints.push_back({ (points[i + 1].x + (points[i + 2].x - points[i + 1].x) / 2),
			(points[i + 1].y + (points[i + 2].y - points[i + 1].y) / 2) });
	}
	//push last 2 points
	pivotPoints.push_back(points[points.size() - 2]);
	pivotPoints.push_back(points[points.size() - 1]);

	//pushback destiny
	pivotPoints.push_back({ 1, destiny});

	return true;
}

void CBeizier::CPoints(fPoint x0, fPoint x1, fPoint x2, fPoint x3)
{
	temp.clear();
	int last = -(x0.x * 100) + (x3.x * 100);
	float delta = (float)100 / last;
	float p = 0;
	for (float ot = 0.f; ot <= 1.f; ot += 0.01f)
	{
		p = (((x0.y)*(1 - ot)*(1 - ot)*(1 - ot)) + 3 * x1.y*ot*(1 - ot)*(1 - ot) + 3 * x2.y*ot*ot*(1 - ot) + x3.y*ot*ot*ot);
		temp.push_back(p);
	}
	for (float count = 0; count < 100; count += delta)
	{
		if (list->size() <= 100)list->push_back(temp[count]);
	}
}

void CBeizier::Bezier(std::vector<fPoint>& points, cbezier_type b_type, int new_destiny)
{
	destiny = new_destiny;

	//clear previous pivots
	pivotPoints.clear();

	switch (b_type)
	{
	case CB_EASE_INOUT_BACK: list = &easeInoutBack;
		break;
	case CB_SLOW_MIDDLE: list = &slowMiddle;
		break;
	case CB_LINEAL: list = &bLineal;
		break;
	case CB_SHAKE: list = &bshake;
		break;
	case CB_FLY: list = &bfly;
		break;
	default: return;
	}

	speed = 0;
	if (points.size() >= 2)
	{
		if (CalculatePivots(points))
		{
			int psize = pivotPoints.size();
			for (int i = 0; i + 3 < psize; i = i + 3)
			{
				CPoints(pivotPoints[i], pivotPoints[i + 1], pivotPoints[i + 2], pivotPoints[i + 3]);
			}
			list->push_back(destiny);
		}
	}
	else if (points.size() == 1)
	{
		float p;
		for (float t = 0.01f; t < 1; t += 0.01f)
		{
			p = 2 * t*(1 - t)*points[0].y + pow(t, 2);
			list->push_back(p);
		}
		list->push_back(destiny);
	}
	else
	{
		float p;
		for (float t = 0.01f; t < 1; t += 0.01f)
		{
			p = t;
			list->push_back(p);
		}
		list->push_back(destiny);
	}
}