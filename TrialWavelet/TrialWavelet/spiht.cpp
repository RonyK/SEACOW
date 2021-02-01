#include <vector>
#include "spiht.h"
#include "iterators.h"

namespace caWavelet
{
	//template<typename Dty_>
	//void caSPIHT<Dty_>::adjustCoor(caCoor<Dty_> coor, 
	//	std::vector<Dty_> sP, std::vector<Dty_> eP)
	//{
	//	if (!isExceed(coor, sP, eP))
	//	{
	//		return;
	//	}

	//	for (size_t i = 1; i < coor.size(); i++)
	//	{
	//		// TODO :: Over
	//		if (coor[i] >= eP[i])
	//		{
	//			coor[i - 1]++;
	//			coor[i] = sP[i];
	//		}
	//	}

	//	for (size_t i = coor.size(); i > 0; i--)
	//	{
	//		// TODO :: Under
	//		if (coor[i] < sP[i])
	//		{
	//			coor[i - 1]--;
	//			coor = eP[i];
	//		}
	//	}
	//}

	// sP <= X < eP
	//template<typename Dty_>
	//bool caSPIHT<Dty_>::isExceed(caCoor<Dty_> coor, std::vector<Dty_> sP, std::vector<Dty_> eP)
	//{
	//	if (sP.size() != eP.size())
	//	{
	//		throw std::domain_error("sP != eP");
	//	}
	//	if (coor.size() > sP.size())
	//	{
	//		throw std::out_of_range("coor > sP, eP");
	//	}

	//	for (size_t i = 0; i < coor.size(); i++)
	//	{
	//		if (coor[i] < sP[i])
	//		{
	//			return false;
	//		}

	//		if (coor[i] >= eP[i])
	//		{
	//			return false;
	//		}
	//	}

	//	return true;
	//}
}

