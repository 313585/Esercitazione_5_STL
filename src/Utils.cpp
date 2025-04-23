#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include "Eigen/Eigen"
#include <string>
#include <list>
#include <map>
#include <vector>


namespace PolygonalLibrary
{

bool ImportMesh(PolygonalMesh& mesh)
{
    // controllo
	if(!ImportCell0Ds(mesh)) return false;

    if(!ImportCell1Ds(mesh)) return false;

    if(!ImportCell2Ds(mesh)) return false;

    return true;
}

bool ImportCell0Ds(PolygonalMesh& mesh)
{
    ifstream ifile("./Cell0Ds.csv");

    if(ifile.fail()) 
		return false;

    list<string> listLines;

    string line;
    while(getline(ifile, line)) 
		listLines.push_back(line);

    ifile.close();

    // rimuovo Id;Marker;X;Y prima riga del file
    listLines.pop_front();

    mesh.NumCell0Ds = listLines.size();

    if (mesh.NumCell0Ds == 0)
    {
        cerr<<" Non c'è cell 0D"<<endl;
        return false;
    }

    mesh.Cell0DsId.reserve(mesh.NumCell0Ds);
    mesh.Cell0DsCoordinates = Eigen::MatrixXd::Zero(3,mesh.NumCell0Ds);

    for(const string& line : listLines)
    {
        istringstream converter(line);

        unsigned int id;
        unsigned int marker;
        char separatore; // ';' 

        converter>>id>>separatore>>marker>>separatore>>mesh.Cell0DsCoordinates(0, id)>>separatore>>mesh.Cell0DsCoordinates(1, id);

        mesh.Cell0DsId.push_back(id);

        // memorizza marker
		if (marker !=0)
		{
			auto it= mesh.MarkerCell0Ds.find(marker);
			if (it !=mesh.MarkerCell0Ds.end())
			{
				(*it).second.push_back(id);
			}
			else
			{
				mesh.MarkerCell0Ds.insert({marker,{id}});
			}
		}
    }
    
    return true;
}


bool ImportCell1Ds(PolygonalMesh& mesh)
{
    ifstream ifile("./Cell1Ds.csv");

    if(ifile.fail()) 
		return false;

    list<string> listLines;

    string line;
    while(getline(ifile, line)) 
		listLines.push_back(line);

    ifile.close();

    mesh.NumCell1Ds = listLines.size();

    // elimino Id;Marker;Origin;End 
    listLines.pop_front();

    if (mesh.NumCell1Ds == 0)
    {
        cerr<<" Non c'è cell 1D"<<endl;
        return false;
    }

    mesh.Cell1DsId.reserve(mesh.NumCell1Ds);
    mesh.Cell1DsExtrema = Eigen::MatrixXi::Zero(2, mesh.NumCell1Ds);

    for(const string& line : listLines)
    {
        istringstream converter(line);

        unsigned int id;
        unsigned int marker;
        char separatore;

        converter>>id>>separatore>>marker>>separatore>>mesh.Cell1DsExtrema(0, id)>>separatore>>mesh.Cell1DsExtrema(1, id);
        mesh.Cell1DsId.push_back(id);

       // memorizza marker
	   if (marker !=0)
	   {
		   auto it=mesh.MarkerCell1Ds.find(marker);
		   if (it !=mesh.MarkerCell1Ds.end())
		   {
			   (*it).second.push_back(id);
		   }
		   else
		   {
			   mesh.MarkerCell1Ds.insert({marker,{id}});
		   }
	   }
    }

    return true;
}

bool ImportCell2D(PolygonalMesh& mesh)
{
    ifstream ifile;
	ifile.open("./Cell2Ds.csv");	

    if(ifile.fail()) 
		return false;

    list<string> listLines;

    string line;
    while(getline(ifile, line)) 
		listLines.push_back(line);
	ifile.close();

    // elimino Id;Marker;NumVertices;Vertices;NumEdges;Edges
    listLines.pop_front();

    mesh.NumCell2Ds = listLines.size();

    if(mesh.NumCell2Ds == 0)
    {
        cerr<<"There is no cell 2D"<<endl;
        return false;
    }

    mesh.Cell2DsId.reserve(mesh.NumCell2Ds);
    mesh.Cell2DsVertices.reserve(mesh.NumCell2Ds);
    mesh.Cell2DsEdges.reserve(mesh.NumCell2Ds);

    for(const string& line : listLines)
    {
        istringstream converter(line);

        unsigned int id;
        unsigned int marker;
        unsigned int num_vert;
        unsigned int num_lati;
        char separatore;

        converter>>id>>separatore>>marker>>separatore>>num_vert;

        vector<unsigned int> vec_v;
        vec_v.reserve(num_vert);
        for(unsigned int i=0; i<num_vert; i++)
        {  
            unsigned int vertice;
            converter>>separatore>>vertice;
            vec_v.push_back(vertice);
        }
        mesh.Cell2DsVertices.push_back(vec_v);


        converter>>separatore>>num_lati;

        vector<unsigned int> vec_l;
        vec_l.reserve(num_lati);
        for(unsigned int j=0; j<num_lati; j++)
        {
            unsigned int lato;
            converter>>separatore>>lato;
            vec_l.push_back(lato);
        }
        mesh.Cell2DsEdges.push_back(vec_l);

        mesh.Cell2DsId.push_back(id);

       // memorizzo marker
	   if (marker !=0)
	   {
		   auto it=mesh.MarkerCell2Ds.find(marker);
		   if (it !=mesh.MarkerCell2Ds.end())
		   {
			   (*it).second.push_back(id);
		   }
		   else
		   {
			   mesh.MarkerCell2Ds.insert({marker,{id}});
		   }
	   }
    }

    return true;
}

}