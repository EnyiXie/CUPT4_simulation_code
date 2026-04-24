
#define PI 3.14159265
#define P1 0.102573
#define P2 -0.068287
#define P3 0.958633
#define P4 0.0407253
#define P5 0.817285



Double_t gaisser(Double_t *x, Double_t *par)
{
  // x[0] = Eu; x[1] = cos(theta);
  // par[0] = gamma + 1
//  double f = 2.*TMath::Pi()*0.14*TMath::Power(x[0],-par[0])*(1.0/(1.0+1.1*x[0]*x[1]/115.)+0.054/(1.0+1.1*x[0]*x[1]/850.));
  double E = x[0];
  double c0=x[1];
  double c1=(c0*c0+P1*P1+P2*pow(c0,P3)+P4*pow(c0,P5))/(1+P1*P1+P2+P4);
  double c2=sqrt(c1);
  double c3=E*(1+3.64/E/pow(c2,1.29));
  double f=2.*TMath::Pi()*0.14*pow(c3,-par[0])*(1.0/(1+1.1*E*c2/115)+0.054/(1+1.1*E*c2/850));


  return f;

}

Double_t lngaisser(Double_t *x, Double_t *par)
{
  // x[0] = ln(Eu); x[1] = cos(theta);
  // par[0] = gamma + 1

  double E = TMath::Exp(x[0]);
  double c0=x[1];
  double c1=(c0*c0+P1*P1+P2*pow(c0,P3)+P4*pow(c0,P5))/(1+P1*P1+P2+P4);
  double c2=sqrt(c1);
  double c3=E*(1+3.64/E/pow(c2,1.29));
  double f=2.*TMath::Pi()*0.14*pow(c3,-par[0])*(1.0/(1+1.1*E*c2/115)+0.054/(1+1.1*E*c2/850));


/*
  double E = TMath::Exp(x[0]);
  double f = 2.*TMath::Pi()*0.14*TMath::Power(E,-par[0])*(1.0/(1.0+1.1*E*x[1]/115.)+0.054/(1.0+1.1*E*x[1]/850.));
*/
  return f*E;

}



void muongun()
{
  std::cout<<"-------start------"<<std::endl;
  TF2 *fcn = new TF2("fcn","lngaisser",TMath::Log(20.),TMath::Log(1000.),0.342,1.,1);
//  double I1[600],I2[600];
  // gamma+1=2.7
  fcn->SetParameter(0,2.7);
  fcn->SetNpx(10000);
  fcn->SetNpy(1000);
  //fcn->Draw();

/*
  // calculate flux
  TF2 *fcn1 = new TF2("fcn1","gaisser",50.,100000.,0.,1.,1);
  fcn1->SetParameter(0,2.7);
  fcn1->SetNpx(10000);
  fcn1->SetNpy(10000);
  std::cout<<"Flux is "<<fcn1->Integral(50.,1000000.,0.,1.,1e-13)<<std::endl;
*/



  double energy,costheta,theta,phi,pos_x,pos_y,Px,Py,Pz;
  double h=140;
  gRandom->SetSeed(0);
  TH1F *h1 = new TH1F("h1","",100000,50,100000);
  TH1F *h2 = new TH1F("h2","",100,0,1);
  std::ofstream ofs;
  ofs.open ("muon.txt", std::ofstream::out | std::ofstream::app);

for (int i=0;i<60000000;i++){
    fcn->GetRandom2(energy,costheta);
    phi=gRandom->Uniform(0,2*TMath::Pi());//产生随机的方位角（概率分布为均匀分布）
    theta=TMath::ACos(costheta);//根据能谱公式抽样结果得到的天顶角
//根据抽样得到的天顶角和方位角，判断缪子的初始位置在属于面源上的那一区域时能被探测器接收到，在这一区域抽样缪子的初始位置
    pos_x=gRandom->Uniform(-0.5-h*tan(theta)*cos(phi),0.5-h*tan(theta)*cos(phi));//初始位置x坐标
    pos_y=gRandom->Uniform(-0.5-h*tan(theta)*sin(phi),0.5-h*tan(theta)*sin(phi)); //初始位置y坐标
    Px=sin(theta)*cos(phi);//速度方向的x坐标
    Py=sin(theta)*sin(phi);//速度方向的y坐标
    Pz=-costheta;//速度方向的z坐标
 //   std::cout<<"-----"<<std::endl;
//    h1->Fill(TMath::Exp(energy));
//    h2->Fill(costheta);
//    if (i==100) std::cout<<i<<std::endl;
    //ofs<<TMath::Exp(energy)<<"  "<<TMath::ACos(costheta)/TMath::Pi()*180.<<"\n";
    ofs<<TMath::Exp(energy)<<"  "<<pos_x<<" "<<pos_y<<" "<<Px<<" "<<Py<<" "<<Pz<<"\n";
//  j++;
  }
//  ofs<<j<<"\n";
  ofs.close();
/*  TCanvas* c1 = new TCanvas("c1","c1",1280,900);
  c1 ->SetLogy();
  c1->Divide(2,1);
  c1->cd(1);
  h1->Draw();
  c1->cd(2);
  h2->Draw();
*/

}
