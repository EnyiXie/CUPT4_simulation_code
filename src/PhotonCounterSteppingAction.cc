//PhotonCounterSteppingAction.cc
#include "PhotonCounterSteppingAction.hh"
#include "PhotonCounterEventAction.hh"
#include "G4Step.hh"
#include "G4OpticalPhoton.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4Track.hh"
#include "Randomize.hh"
#include "G4VProcess.hh"
#include "G4OpBoundaryProcess.hh" 
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
PhotonCounterSteppingAction::PhotonCounterSteppingAction(PhotonCounterEventAction* eventAction)
: G4UserSteppingAction(),
  fEventAction(eventAction)
{}

PhotonCounterSteppingAction::~PhotonCounterSteppingAction()
{}

G4double GetQuantumEfficiency() {
    // 返回量子效率。这里简化为常数，实际上可能需要根据波长插值一个表格。
    const G4double QE = 0.25; // 假设25%的量子效率
    return QE;
}

G4double GenerateSPECharge() {
    // 假定单光电子电荷以某个均值和标准偏差的高斯分布，可以根据实测数据进行调整
    const G4double meanCharge = 192.5; // 单光电子的平均输出电荷(fC), 假设量程25fC
    const G4double stdDevCharge = 75.7; // 标准偏差
    return G4RandGauss::shoot(meanCharge, stdDevCharge);
}


void PhotonCounterSteppingAction::UserSteppingAction(const G4Step* step)
{
    
   G4Track* track = step->GetTrack(); //每一步都有一个主角（粒子），这行代码是问系统：“现在正在跑的这个家伙是谁？”

    // --- 逻辑 1: 检查并记录闪烁光子的产生  ---
    //这部分代码就是缪子穿过塑闪会产生光子，通过统计所有光子的数目，来记录总共产生了多少光子
    const std::vector<const G4Track*>* secondaries = step->GetSecondaryInCurrentStep();
    if (secondaries->size() > 0) {
        for (unsigned int i = 0; i < secondaries->size(); ++i) {
            if ((*secondaries)[i]->GetDefinition() == G4OpticalPhoton::Definition()) {
                const G4VProcess* creatorProcess = (*secondaries)[i]->GetCreatorProcess();
                if (creatorProcess && creatorProcess->GetProcessName() == "Scintillation") {
                    fEventAction->AddGeneratedPhoton(); // 统计“总共产生了多少光子”的地方。
                }
            }
        }
    }
// --- 逻辑 2: 检查光学光子是否命中PMT ---
    if(track->GetDefinition() == G4OpticalPhoton::Definition())
    {

        // 出了世界边界，这个 volume 就是 nullptr，直接 return。

        G4StepPoint* postPoint = step->GetPostStepPoint(); // 获取后一点 (PostStepPoint)
        if (!postPoint) return; //这一步的终点在哪？如果没终点，直接撤，防止报错。

        G4VPhysicalVolume* volume = postPoint->GetPhysicalVolume(); //获取后一点的物理体 (Physical Volume)
        if (!volume) return;  //如果是空的，说明飞出去了，别管了，直接return

        G4String volName = volume->GetName(); // 既然没飞出去，那你告诉我撞到的这个东西叫啥名字？把名字命名为volName

        // 1. 判断光子是否在 PMT 肚子里
        // 如果 Geant4 算了全反射，光子会弹回塑闪，volName 就是塑闪的名字，不会进这个 if。
        // 只要进了这个 if，说明光子已经物理上“透射”进来了。
        if (volName == "physCornerPMT1" || volName == "physCornerPMT2" ||
            volName == "physCornerPMT3" || volName == "physCornerPMT4") 
        {
            // 2. 手动模拟量子效率 (QE)
            // 只有进来的光子才有资格抽奖。假设效率是 23%
            G4double quantumEfficiency = 0.23; 
            // 摇骰子：0.0 到 1.0 之间的随机数
            if (G4UniformRand() < quantumEfficiency) 
            {
                // --- 情况 A：中奖了 (探测) ---
                // 记录是哪个 PMT 探测的
                if (volName == "physCornerPMT1") 
                    fEventAction->AddTransmittedPhoton1(track->GetTrackID());
                else if (volName == "physCornerPMT2") 
                    fEventAction->AddTransmittedPhoton2(track->GetTrackID());
                else if (volName == "physCornerPMT3") 
                    fEventAction->AddTransmittedPhoton3(track->GetTrackID());
                else if (volName == "physCornerPMT4") 
                    fEventAction->AddTransmittedPhoton4(track->GetTrackID());
                
                // 探测到了，任务完成。
            }
            
            else 
            {
                // --- 情况 B：没中奖 (吸收) ---
                // 变成热量损耗掉了。
                // 这里什么都不用写，不计数就是了。
            }

            // 3. ★★★ 终极保险：光子必须死 ★★★
            // 无论刚才有没有被探测，光子既然进了 PMT (被光阴极捕获)，
            // 就绝对不能让它活着穿出去。直接杀掉。
            track->SetTrackStatus(fStopAndKill);
        }
    }
    

    // --- 逻辑 3: 检查能量沉积 (保持不变) ---
    G4LogicalVolume* preVolume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
    if (preVolume->GetName() == "logicShape") {
        G4double edep = step->GetTotalEnergyDeposit();
        if (track->GetDefinition() != G4OpticalPhoton::Definition()) {
            if (edep > 0.) {
                fEventAction->AddEdep(edep);
            }
        }
    }
}
