import { Component, OnInit } from '@angular/core';
import { ExaminationsService } from '../services/examinations.service';
import { Router } from '@angular/router';
import { AllExamination } from '../models/all_examination';
import { Specialization } from '../models/specialization';

@Component({
  selector: 'app-manager-examinations',
  templateUrl: './manager-examinations.component.html',
  styleUrls: ['./manager-examinations.component.css']
})
export class ManagerExaminationsComponent implements OnInit {

  constructor(private router: Router, private examinationService: ExaminationsService) { }

  newExaminations: AllExamination[] = [];
  specializations: Specialization[] = [];

  ngOnInit(): void {
    let type = localStorage.getItem("type");
    if(type != 'manager') this.router.navigate(['']);
    let username = localStorage.getItem("user");
    username = username == null ? "" : username; // check if username wasnt set
    if(username == "") this.logout;
    this.examinationService.getNewExaminations().subscribe((data: AllExamination[])=>{
      this.newExaminations = data;
    })
    this.examinationService.getAllSpecializations().subscribe((data: Specialization[])=>{
      this.specializations = data;
    })
  }

  logout() {
    localStorage.clear()
    this.router.navigate([''])
  }

  message: string;

  addExamination(examination,specialization) {
    this.examinationService.acceptExamination(examination,specialization).subscribe(respObj=>{
      if(respObj['message']=='ok'){
        this.message = 'Successfully added examination'
        this.examinationService.getNewExaminations().subscribe((data: AllExamination[])=>{
          this.newExaminations = data;
        })
        alert(this.message)
      }
      else{
        this.message = 'Error'
        alert(this.message)
        
      }
    });
  }

  removeExamination(examination,specialization) {
    this.examinationService.removeExamination(examination,specialization).subscribe(respObj=>{
      if(respObj['message']=='ok'){
        this.message = 'Successfully removed examination'
        this.examinationService.getNewExaminations().subscribe((data: AllExamination[])=>{
          this.newExaminations = data;
        })
        alert(this.message)
      }
      else{
        this.message = 'Error'
        alert(this.message)
        
      }
    });
  }

  newSpecialization: string;
  isSpecializationValid = true;
  oldSpecialization = false;

  addSpecialization(): void {

  // Validate other selections
  this.isSpecializationValid = !!this.newSpecialization;
  this.oldSpecialization = false;
  for(let i=0; i < this.specializations.length; i++){
    if(this.newSpecialization == this.specializations[i].specialization){
      this.oldSpecialization = true;
      break;
    }
  }

  // Check if all inputs are valid
  if (this.isSpecializationValid && !this.oldSpecialization) {

      this.examinationService.addSpecialization(this.newSpecialization).subscribe(respObj=>{
        if(respObj['message']=='ok'){
          this.examinationService.getAllSpecializations().subscribe((data: Specialization[])=>{
            this.specializations = data;
          })
          let m = 'Successfully added specialization'
          alert(m)
        }
        else{
          let m = 'Error'
          alert(m)
          
        }
      });
  }
  else{
    alert('Please fix the form errors before submitting.');
    }
  }


  specialization: string = '';
  examinations: AllExamination[] = [];

  getExaminations():void {
    if(this.specialization == '') return ;
    this.examinationService.getSpecExaminations(this.specialization).subscribe((data: AllExamination[])=>{
      this.examinations = data;
    })
  }

  deleteExamination(examination,specialization) {
    this.examinationService.deleteExamination(examination,specialization).subscribe(respObj=>{
      if(respObj['message']=='ok'){
        this.message = 'Successfully deleted examination'
        this.examinationService.getSpecExaminations(this.specialization).subscribe((data: AllExamination[])=>{
          this.examinations = data;
        })
        alert(this.message)
      }
      else{
        this.message = 'Error'
        alert(this.message)
        
      }
    });
  }

  examination: string;
  duration: number;
  price: string;

  updateExamination(exam, dur, pr): void{

    if(this.examination == '') this.examination=exam;
    if(!this.duration) this.duration=dur;
    if(this.duration < 0) this.duration = 30; // ukoliko se unese da je trajanje nula
    if(this.price == '') this.price=pr;

    this.examinationService.updateExamination(exam,this.examination,this.specialization,this.duration,this.price).subscribe(respObj=>{
      if(respObj['message']=='ok'){
        this.message = 'Successfully updated examination'
        this.examinationService.getSpecExaminations(this.specialization).subscribe((data: AllExamination[])=>{
          this.examinations = data;
        })
        alert(this.message)
      }
      else{
        this.message = 'Error'
        alert(this.message)
        
      }
    });

  }

  validateIntegerInput(event: any): void {
    if ((event.which < 48 || event.which > 57) && event.which !== 8) {
      event.preventDefault();
    }
  }

  newExamination: string;
  addDuration: number;
  addPrice: string;

  // Initialize boolean flags for input validation
  isExaminationValid = true;
  isPriceValid = true;

  addNewExamination(): void {

  // Validate other selections
  this.isExaminationValid = !!this.newExamination;
  this.isPriceValid = !!this.addPrice;
  if(!this.addDuration) this.addDuration = 30;

  // Check if all inputs are valid
  if (this.isExaminationValid && this.isPriceValid && (this.addDuration > 0)) {

      this.examinationService.addExamination(this.newExamination,this.specialization,this.addDuration,this.addPrice).subscribe(respObj=>{
        if(respObj['message']=='ok'){
          this.message = 'Successfully added examination'
          this.examinationService.getSpecExaminations(this.specialization).subscribe((data: AllExamination[])=>{
            this.examinations = data;
          })
          alert(this.message)
        }
        else{
          this.message = 'Error'
          alert(this.message)
          
        }
      });
  }
  else{
    alert('Please fix the form errors before submitting.');
    }
  }

}
