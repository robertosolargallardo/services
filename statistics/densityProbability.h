/*
Funciones de densidad de probabilidad
*/
#if !defined(_DENSITYPROBABILITY_H)
#define _DENSITYPROBABILITY_H
//Libs C
#include <stdio.h>
#include <math.h>       /*ej: exp */
//Libs STL
#include <algorithm>
#include <iostream>
#include <list>
#include <vector>
#include <set>
#include <map>
//Libs extras
#include "stats.h"
#define END_OF_STREAM 999999999
using namespace std;


class FuncionDensidad{
public:
    
    //Data de entrada
    vector<double> sample;
    //Data de entrada normalizada
    vector<double> sampleNormalized;

    
    /***Distribución de densidad posterior***/
     //Base en escala real (solo para data gráfica)
    vector<double> samplePost;
    //Normalizada base
    vector<double> samplePostNormalized; 
    //Normalizada base escalada en razon de la Std
    vector<double> samplePostNormalizedScaled; 
    
    //Distribución de probabilidad Normal
    vector<double> distProbabilityNormal; 



    //Mediana, promedio, Std y Varianza
    double sampleMediana;
    double samplePromedio;
    double sampleStd;
    double sampleVariance;

    //Mediana, promedio, Std y Varianza, todos normalizados
    double sampleMedianaNormalized;
    double sampleVarianceNormalized;
    double sampleStdNormalized;
    double samplePromedioNormalized;

    //Tipo de función de densidad
    //tipoFD=1 -> Normal
    //tipoFD=2 -> Binomial
    // ...
    // etc
    int tipoFD; 

    FuncionDensidad(){

    }

    ~FuncionDensidad(){
        
    }

    void computeFuncionDensidad(vector<double> sampleIn){
    //Sort para distribuciones y uso posterior sin necesidad de ordenar más de una vez     
    sort(sampleIn.begin(), sampleIn.end()); 
    sample = sampleIn;
    sampleNormalized = normalizedData(sampleIn);
        
    //Valores de entrada
    sampleMediana = mediana(sampleIn);
    sampleVariance = varianza(sampleIn);
    sampleStd = sqrt(sampleVariance);
    samplePromedio = promedio(sampleIn);

    //Valores normalizados
    sampleMedianaNormalized = mediana(sampleNormalized);
    sampleVarianceNormalized = varianza(sampleNormalized);
    sampleStdNormalized = sqrt(sampleVarianceNormalized);
    samplePromedioNormalized = promedio(sampleNormalized);

    //Obención vector distribución normal
    outPostNormal();
    //Obención vector probabilidad
    outProbabilidadNormal();
    }

  

    /**** Distribution Normal y funciones asociadas****/
    void outPostNormal()
    {
    //vector<double> dataOut;
    double zTmp,value;

    for (std::vector<double>::iterator it = sampleNormalized.begin() ; it != sampleNormalized.end(); ++it)
    {
        zTmp = zValue((*it),sampleMedianaNormalized,sampleStdNormalized);
        value = kernelNormal(zTmp);
        samplePostNormalizedScaled.push_back(value/sampleStdNormalized);
        samplePostNormalized.push_back(value);
    }


    }

    void outProbabilidadNormal()
    {
    double Pr;
        for (std::vector<double>::iterator it = sampleNormalized.begin() ; it != sampleNormalized.end(); ++it)
        {
            Pr = probabilidadNormal((*it),sampleMedianaNormalized,sampleStdNormalized);
            distProbabilityNormal.push_back(Pr);
        }
    }



    //Kernel que retorna la Probabilidad de  Pr(X<x) 
    //sobre una distribución de densidad Normal 
    double probabilidadNormal(double x, double medianaIn, double desviationStd)
    {
        return 0.5*(1.0 + erfPr(zValue(x,medianaIn,desviationStd)/sqrtOfTwo));
    }

    //Kernel que retorna la Probabilidad de  Pr(a<Pr(x)<b) en [a,b]
    //sobre una distribución de densidad Normal 
    double probabilidadNormalAB(double medianaIn, double desviationStd, double a, double b)
    {
        double intA = probabilidadNormal(a, medianaIn, desviationStd) ;
        double intB = probabilidadNormal(b, medianaIn, desviationStd) ;
        return (intB - intB);
    }

    //Aproximación integral de probabilidad normal, como serie de potencias para reducir computo
    //Se deben utilizar valores normalizados entre [0,1]
    double erfPr(double x)
    {
        return (2.0/sqrtOfPi)*(x - pow(x,3)/3.0  + pow(x,5)/10.0 - pow(x,7)/42.0 + pow(x,9)/216.0 - pow(x,11)/1320.0 + pow(x,13)/9360.0);
    }

    //Valor Z usado en distribución Normal
    double zValue(double x, double medianaIn, double desviationStn)
    {return (x - medianaIn)/desviationStn;}

    //Kernel Normal para mapeo de valores en función de distribucion
    //phi=@(x)(exp(-.5*x.^2)/sqrt(2*pi));
    double kernelNormal(double x){
    return (exp( (-0.5)*pow(x,2.0) )/sqrtOf2Pi);
    }
     /**************************************/


    /****** Function Epanechnikov *********/ 
    //Kernel Epanechnikov 
    //epa=@(x)3/4*(1-x.^2), condicion |x|<=1
    double kernelEpanechnikov(double x){
    return 0.75*(1.0 - pow(x,2.0));
    }
    /**************************************/


    /****** Function Uniforme ****/    
    //Kernel Uniforme
    //uni=@(x)1/2,  condicion |x|<=1
    double kernelUniform(double x){
    return 0.5;
    }


};

#endif
