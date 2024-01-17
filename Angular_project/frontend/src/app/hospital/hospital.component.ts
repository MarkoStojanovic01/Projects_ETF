import { Component, OnInit } from '@angular/core';
import { UserService } from '../services/user.service';
import { User } from '../models/user';

@Component({
  selector: 'app-hospital',
  templateUrl: './hospital.component.html',
  styleUrls: ['./hospital.component.css']
})
export class HospitalComponent implements OnInit {

  constructor(private userService: UserService) { }

  allDoctors: User[] = []
  shownDoctors: User[] = []
  images: string[] = [
    '/assets/dunder-Mifflin-building.jpg',
    '/assets/Hospital_Recepcion.jpg',
    '/assets/Hospital_Badroom.jpg',
    '/assets/Hospital_OperationRoom.jpg',
    '/assets/Hospital_Garden.png'
  ]
  image: string
  inedx: number = 0 // image index

  ngOnInit(): void {
    this.userService.getAllDoctors().subscribe((data:User[])=>{
      this.allDoctors = data;
      this.shownDoctors = data;
    })
    this.image = this.images[this.inedx];
  }

  // Sorting properties
  private sortDirection = 1; // 1 for ascending, -1 for descending
  private currentSortKey = '';

  sortBy(key: string): void {
    this.sortDirection = 1;
    this.currentSortKey = key;
    this.shownDoctors.sort((a, b) =>
      a[key].localeCompare(b[key]) * this.sortDirection
    );
  }

  reverseSortBy(key: string): void {
    this.sortDirection = -1;
    this.currentSortKey = key;
    this.shownDoctors.sort((a, b) =>
      a[key].localeCompare(b[key]) * this.sortDirection
    );
  }

  // Search text object
  searchText = {
    firstname: '',
    lastname: '',
    specialization: ''
  };

  get filteredDoctors(): User[] {
    return this.allDoctors.filter(doc =>
      (doc.firstname.toLowerCase().includes(this.searchText.firstname.toLowerCase()) &&
      doc.lastname.toLowerCase().includes(this.searchText.lastname.toLowerCase()) &&
      doc.specialization.toLowerCase().includes(this.searchText.specialization.toLowerCase()))
    );
  }

  applyFilters(): void {
    this.shownDoctors = this.filteredDoctors
  }

  left():void{
    this.inedx = this.inedx==0 ? this.images.length-1 : this.inedx-1;
    this.image = this.images[this.inedx];
  }

  right():void{
    this.inedx = this.inedx==this.images.length-1 ? 0  : this.inedx+1;
    this.image = this.images[this.inedx];
  }

  

}
