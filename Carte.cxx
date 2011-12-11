// This example creates a polygonal model of a cone, and then renders it to
// the screen. It will rotate the cone 360 degrees and then exit. The basic
// setup of source -> mapper -> actor -> renderer -> renderwindow is 
// typical of most VTK programs.
//

// First include the required header files for the VTK classes we are using.
#include "vtkImageData.h"
#include "vtkPolyDataMapper.h"
#include "vtkContourFilter.h"
#include "vtkRenderWindow.h"
#include "vtkCamera.h"
#include "vtkActor.h"
#include "vtkRenderer.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkRenderWindowInteractor.h"
#include <boost/numeric/ublas/matrix.hpp>
#include "Map.hpp"

using namespace boost::numeric::ublas;


vtkImageData * MatrixToVtkImageData(const Map & map) {
 
   const matrix<double> & m = map.elevation;
   vtkImageData * p = vtkImageData::New();

   p->SetDimensions(map.nrows, map.ncols,1);
   p->SetNumberOfScalarComponents(1);
   p->SetSpacing(map.cellsize, map.cellsize, 0.);
   p->SetOrigin(map.xllcorner, map.yllcorner, 0.);

   int * dims = p->GetDimensions();
   for(int i = 0; i < dims[0]; i++) { 
      for (int j = 0; j < dims[1]; ++j){
       p->SetScalarComponentFromDouble(i,j,0,0,m(i,j));
      }
   }
}

int main(int argc, char *argv[]) {
  Map m(argv[1],48.2998,6.935,48.312,6.950);
  vtkImageData * carte = MatrixToVtkImageData(m); 
  
  vtkContourFilter *contour = vtkContourFilter::New();
      contour->SetInput(carte);            //original
 
  double min_ele = -500;
  double max_ele = 9000;
  int step = 25;
  int num = static_cast<int>((max_ele-min_ele)/step);
  

  contour->GenerateValues(num, min_ele, max_ele);   
  vtkPolyDataMapper *carteMapper = vtkPolyDataMapper::New();
      carteMapper->SetInputConnection(contour->GetOutputPort());
      carteMapper->ScalarVisibilityOff();
  vtkActor *carteActor = vtkActor::New();
      carteActor->SetMapper(carteMapper);
 
  vtkRenderer *ren1= vtkRenderer::New();
  ren1->AddActor( carteActor );
  ren1->SetBackground( 0.1, 0.2, 0.4 );

  vtkRenderWindow *renWin = vtkRenderWindow::New();
  renWin->AddRenderer( ren1 );
  renWin->SetSize( 300, 300 );
   // we define an WindowInteractor
  vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
  iren->SetRenderWindow( renWin);
  vtkInteractorStyleTrackballCamera *style = vtkInteractorStyleTrackballCamera::New();
  iren->SetInteractorStyle(style);


  //
  // Free up any objects we created. All instances in VTK are deleted by
  // using the Delete() method.
  //
  
  iren->Initialize();
  iren->Start();


  
  carte->Delete();
  contour->Delete();
  carteMapper->Delete();
  carteActor->Delete();
  ren1->Delete();
  
  renWin->Delete();
  iren->Delete();
  style->Delete();

  return 0;
}


